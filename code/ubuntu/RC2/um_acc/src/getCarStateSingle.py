#!/usr/bin/env python

import rospy
from std_msgs.msg import String, UInt16, Float64
from um_acc.msg import Motor_data, IMU, CarState
import tf
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Point, Pose, Quaternion, Twist, Vector3
from sensor_msgs.msg import LaserScan

from scipy.integrate import ode
import scipy.signal as signal 
import numpy as np
import math

def getLocalCoordinates(X, Y, yaw):
    y0 = 0
    th_correct = 0
    # if X < 16:
    y0 = X/48*0.75
    th_correct = np.arctan(0.75/48)
    # elif X < 36 and X >= 16:
    #     y0 = 0.5 + (X-16)/20*0.15
    #     th_correct = np.arctan(0.15/20)
    # else:
    #     y0 = 0.65 + (X-36)/20*0.15
    #     th_correct = np.arctan(0.15/20)
    
    # if (mode == 0):
    #     y = -(Y-y0) + Y_mark
    # else:
    #     y = y_obstacle
    phi = -(yaw-th_correct)
    rd = 0
    return -(Y-y0), phi, rd

def motor_callback(data):
    global u
    u = 0.004641 * data.countPerSecond

def IMU_callback(data):
    global r
    r = data.gz*np.pi/180.0
    # print(w)

def v_callback(data):
    global v
    v = data.data
    # print(data.data)

mode = 0
x_obstacle = 0
y_obstacle = 0
Y_mark = 0
X = 0.0
Y = 0.0
yaw = 0.0
h = 8.0
vl = 0.0
u = 0.0
y = 0.0
v = 0.0
phi = 0.0
r = 0.0
rd = 0.0

rospy.init_node('getCarStateSingle2')
carState_pub = rospy.Publisher("CarState2", CarState, queue_size=10)
# command_pub = rospy.Publisher("command", String, queue_size=10)
listener = tf.TransformListener()
# rospy.Subscriber("Motor_data1", Motor_data, motor_callback1)
rospy.Subscriber("Motor_data2", Motor_data, motor_callback)
rospy.Subscriber("IMU_data2", IMU, IMU_callback)
rospy.Subscriber("lateral_v2", Float64, v_callback)

rate = rospy.Rate(10.0)
while not rospy.is_shutdown():
    try:
        (trans,quaternion) = listener.lookupTransform('/map', '/base_link', rospy.Time(0))
    except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
        continue

    X = trans[0]
    Y = trans[1]
    # if X > 28:
    #     vl = 0
    #     h = np.minimum(36 - X, h)

    roll, pitch, yaw = tf.transformations.euler_from_quaternion(quaternion)
    # print rospy.Time.now().to_sec(), "X:", X, "Y:", Y, "Z:", trans[2], "roll:", roll, "pitch:", pitch, "yaw:", yaw
    y, phi, rd = getLocalCoordinates(X, Y, yaw)
    msg = CarState()
    msg.time = rospy.Time.now().to_sec()
    msg.x_global = X
    msg.y_global = Y
    msg.yaw_global = yaw
    msg.h = h
    msg.vl = vl
    msg.u = u
    msg.y = y
    msg.v = v
    msg.phi = phi
    msg.r = r
    msg.rd = rd
    carState_pub.publish(msg)

    rate.sleep()