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
import matplotlib.pyplot as plt

# class butterworth:
#     # the A, B, C, D matrix are not correct, because tf2ss is only for continuous system
#     A, B, C, D, x = [], [], [], [], []
#     def __init__(self, cutoff_frequnecy, sampling_frequency, order = 2):
#         b, a = signal.butter(order, cutoff_frequnecy/(sampling_frequency/2.0), btype='low', analog=False, output='ba')
#         self.x = np.zeros((order, 1))
#         self.A, self.B, self.C, self.D = signal.tf2ss(b,a)
#         print 'A:', self.A, 'B:', self.B, 'C:', self.C, 'D:', self.D

#     def filter(self, newValue):
#         self.x = self.A.dot(x) + self.B*newValue
#         return self.C.dot(self.x) + self.D*newValue

class butterworth:
    A = np.matrix([[-0.828790589025407, -0.451442114904200],[0.451442114904200,   -0.190355027501495]])
    B = np.matrix([[0.638435561523912],[1.68341622393161]])
    C = np.matrix([[0.159608890380978,  0.286252725203644]])
    D = 0.595177513750748
    x = np.matrix([[0],[0]])

    def __init__():
        print('A butterworth fiter with cutoff_frequnecy of 5 hz and sampling rate of 13 hz.')

    def filter(self, newValue):
        self.x = self.A.dot(x) + self.B*newValue
        return self.C.dot(self.x) + self.D*newValue


def inBox(points, x_range, y_range):
    cond1 = (points[:, 0] > x_range[0])
    cond2 = (points[:, 0] < x_range[1])
    cond3 = (points[:, 1] > y_range[0])
    cond4 = (points[:, 1] < y_range[1])
    return np.all(np.vstack((cond1, cond2, cond3, cond4)), axis = 0)


def getLocalCoordinates(X, Y, yaw):
    y0 = 0
    th_correct = 0
    y0 = X/48*0.75
    th_correct = np.arctan(0.75/48)
    # if (mode == 0):
    #     y = -(Y-y0) + Y_mark
    # else:
    #     y = y_obstacle
    phi = -(yaw-th_correct)
    rd = 0
    return -(Y-y0), phi, rd

def motor_callback1(data):
    global vl
    vl = 0.004641 * data.countPerSecond
    frontCar = True

def motor_callback2(data):
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

def lidar_callback(data):
    plotPoint = False # Note that plotting the points out with matplotlib causes significant delay
    box_w, box_l = 0.5, 0.7
    global mode, h, yaw, y, x_obstacle, y_obstacle, u, vl, h_prev, t_prev #, Y_mark
    n = len(data.ranges)
    th = np.linspace(data.angle_min, data.angle_max, n) + yaw
    print rospy.Time.now().to_sec(), th[n-1], data.angle_max
    point_x = data.ranges*np.cos(th);
    point_y = data.ranges*np.sin(th);
    point_data = np.vstack((point_x, point_y, th, np.array(data.ranges))).T
    print 'yaw:', yaw*180/np.pi, 'deg, y:', y, 'mode:', mode
    print 'x_obstacle', x_obstacle, 'y_obstacle', y_obstacle
    if (mode == 0 and (np.absolute(yaw) < 10.0/180.0*np.pi) and (np.absolute(y) < 0.05)):  #: # and 
        detect_range = 5.0/180.0*np.pi;
        condition = np.all(np.vstack((point_data[:,2]>=-detect_range, point_data[:,2]<=detect_range)), axis = 0)
        point_ahead = point_data[condition, :]
        print 'point_ahead', point_ahead
        condition = (point_ahead[:,3] < 4.0)
        if np.any(condition):
            point_ahead = point_ahead[condition,:]
            print(point_ahead)
            print 'point_ahead.shape', point_ahead.shape
            mode = 1
            x_obstacle = np.mean(point_ahead[:,0])
            y_obstacle = np.mean(point_ahead[:,1])

    if mode == 0:
        h = 8.0
    if mode >= 1:
        x_range = (x_obstacle - box_l/2, x_obstacle + box_l/2)
        y_range = (y_obstacle - box_w/2, y_obstacle + box_w/2)
        condition = inBox(point_data, x_range, y_range)
        obs_points = point_data[condition, :]
        if obs_points.shape[0] > 0:
            x_obstacle = np.mean(obs_points[:,0])
            y_obstacle = np.mean(obs_points[:,1])
            h = x_obstacle
            # msg = String()
            # msg.data = "Go"
            # command_pub.publish(msg)
            # Y_mark = y_obstacle
        else:
            x_obstacle, y_obstacle, h = 0, 0, 8.0
            mode = 0

    if not frontCar:
        vl = u + (h - h_prev)/(rospy.Time.now().to_sec() - t_prev)

    t_prev = rospy.Time.now().to_sec()
    h_prev = h
    if plotPoint == True:
        plt.clf()
        plt.plot(point_x.T, point_y.T, 'b.')
        if not mode == 0:
            boxline_x = (x_obstacle-box_l/2, x_obstacle+box_l/2, x_obstacle+box_l/2, x_obstacle-box_l/2, x_obstacle-box_l/2)
            boxline_y = (y_obstacle-box_w/2, y_obstacle-box_w/2, y_obstacle+box_w/2, y_obstacle+box_w/2, y_obstacle-box_w/2)
            plt.plot(boxline_x, boxline_y, 'r')
            plt.plot(obs_points[:,0], obs_points[:,1], 'r.')
            plt.plot(x_obstacle, y_obstacle, 'ko')
        plt.axis("equal")
        plt.draw()
        plt.pause(0.00001)

# h_filter = butterworth(5, 13, order=2)
mode = 0
x_obstacle = 0
y_obstacle = 0
# Y_mark = 0
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
frontCar = False
h_prev = 8.0
plt.ion()
plt.show()

rospy.init_node('getCarStateDouble')
t_prev = rospy.Time.now().to_sec();
carState_pub = rospy.Publisher("CarState2", CarState, queue_size=10)
# command_pub = rospy.Publisher("command", String, queue_size=10)
listener = tf.TransformListener()
rospy.Subscriber("Motor_data1", Motor_data, motor_callback1)
rospy.Subscriber("Motor_data2", Motor_data, motor_callback2)
rospy.Subscriber("IMU_data2", IMU, IMU_callback)
rospy.Subscriber("lateral_v2", Float64, v_callback)
rospy.Subscriber("scan", LaserScan, lidar_callback, queue_size=1)

rate = rospy.Rate(10.0)
while not rospy.is_shutdown():
    try:
        (trans,quaternion) = listener.lookupTransform('/map', '/base_link', rospy.Time(0))
    except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
        continue

    X = trans[0]
    Y = trans[1]
    if X > 40:
        vl = 0
        h = np.minimum(50 - X, h)

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