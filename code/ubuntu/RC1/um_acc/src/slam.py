#!/usr/bin/env python

import rospy
from std_msgs.msg import String, UInt16, Float64
from um_acc.msg import Motor_data
from um_acc.msg import IMU
import tf
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Point, Pose, Quaternion, Twist, Vector3

from scipy.integrate import ode
import numpy as np

def f(t, y, u, r, steer):
    '''
    y[0] = X, dX/dt = u*cos(theta)-v*sin(theta)
    y[1] = Y, dY/dt = u*sin(theta)+v*cos(theta)
    y[2] = theta
    y[3] = v when the direction of u is aligned with X, v is in the opposite direction of Y
    '''
    a = 0.17
    b = 0.16
    Caf = 14.66
    Car = Caf*a/b
    Iz = 0.03332

    if u > 0.1:
        dvdt = -(Caf+Car)/u*y[3] -r*((b*Car - a*Caf)/u - u) + Caf*steer
    elif u <= 0.1 and u >= -0.1:
        dvdt = 0
    else:
        dvdt = (Caf+Car)/u*y[3] +r*((b*Car - a*Caf)/u - u) + Caf*steer

    # put a negative sign in front of y[3] because v is in opposite direction of Y
    dXdt = u*np.cos(y[2]) + y[3]*np.sin(y[2])
    dYdt = u*np.sin(y[2]) - y[3]*np.cos(y[2])

    return [dXdt, dYdt, r, dvdt]

def motor_callback(data):
    global prev_time, Y_prev, ODE45
    global u
    u = 0.004641 * data.countPerSecond
    global r
    rospy.loginfo("velocity: %fm/s, omega: %sdeg", u, r*180.0/np.pi)
    print(r)
    if u < 0.01:
        Y_prev[3] = 0 # set v = 0
    ODE45.set_initial_value(Y_prev, (prev_time - start_time).to_sec())
    ODE45.set_f_params(u, r, steer)
    
    current_time = rospy.Time.now()
    t = (current_time - start_time).to_sec()
    Y = ODE45.integrate(t)
    x = Y[0]
    y = Y[1]
    th = Y[2]
    print 'x:', x, 'y:', y, 'th:', th, 'v:', Y[3]
    prev_time = current_time
    Y_prev = Y

    # since all odometry is 6DOF we'll need a quaternion created from yaw
    odom_quat = tf.transformations.quaternion_from_euler(0, 0, th)

    # first, we'll publish the transform over tf
    odom_broadcaster.sendTransform(
        (x, y, 0.),
        odom_quat,
        current_time,
        "base_link",
        "odom"
    )

    v = Float64()
    v.data = Y[3]
    v_pub.publish(v)

    # # next, we'll publish the odometry message over ROS
    # odom = Odometry()
    # odom.header.stamp = current_time
    # odom.header.frame_id = "odom"

    # # set the position
    # odom.pose.pose = Pose(Point(x, y, 0.), Quaternion(*odom_quat))

    # # set the velocity
    # odom.child_frame_id = "base_link"
    # odom.twist.twist = Twist(Vector3(vx, vy, 0), Vector3(0, 0, vth))

    # # publish the message
    # odom_pub.publish(odom)


def IMU_callback(data):
    global r
    r = -data.gz*np.pi/180.0
    # print(w)

def servo_callback(data):
    global steer
    servoMid = 1537
    steer = (data.data - servoMid)*8.9479*10**(-4)
    # print(data.data)

ODE45 = ode(f).set_integrator('dopri5')
u = 0.0
r = 0.0
steer = 0.0

Y0 = [0,0,0,0]
t0 = 0
Y_prev = [0,0,0,0]

rospy.init_node('slam')
start_time = rospy.Time.now()
prev_time = rospy.Time.now()
odom_broadcaster = tf.TransformBroadcaster()
odom_pub = rospy.Publisher("odom", Odometry, queue_size=50)
v_pub = rospy.Publisher("lateral_v2", Float64, queue_size=10)
rospy.Subscriber("Motor_data2", Motor_data, motor_callback)
rospy.Subscriber("IMU_data2", IMU, IMU_callback)
rospy.Subscriber("servo2", UInt16, servo_callback)
listener = tf.TransformListener()

ODE45.set_initial_value(Y0, t0).set_f_params(u, r, steer)

rate = rospy.Rate(1.0)
while not rospy.is_shutdown():
    try:
        (trans,quaternion) = listener.lookupTransform('/map', '/base_link', rospy.Time(0))
    except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
        continue

    prev_time = rospy.Time.now()
    Y_prev[0] = trans[0]
    Y_prev[1] = trans[1]
    roll, pitch, yaw = tf.transformations.euler_from_quaternion(quaternion)
    Y_prev[3] = yaw
    print "acml : x: ", Y_prev[0], " y: ", Y_prev[1], " z: ", Y_prev[2]
    # if X > 28:
    #     vl = 0
    #     h = np.minimum(36 - X, h)
    rate.sleep()