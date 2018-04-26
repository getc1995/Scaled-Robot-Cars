#!/usr/bin/env python

import rospy
from std_msgs.msg import String, UInt16, Float32, Float64
from um_acc.msg import Motor_data, IMU, CarState, CentralSignal
from um_acc.srv import MPC_CC, MPC_LK, MPC_ACC
import tf
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Point, Pose, Quaternion, Twist, Vector3
from sensor_msgs.msg import LaserScan

from scipy.integrate import ode
import numpy as np
import math
import matplotlib.pyplot as plt
from getLatDynMat import *

def CarState_callback(data):
    global X, Y, yaw, h, vl, u, y, v, phi, r, rd
    X = data.x_global
    Y = data.y_global
    yaw = data.yaw_global
    h = data.h
    vl = data.vl
    u = data.u
    y = data.y
    v = data.v
    phi = data.phi
    r = data.r
    rd = data.rd

def Control_callback(data):
    global manual_steer, right_lane, manual_v, stop, command_v
    manual_steer = data.manual_steer
    right_lane = data.right_lane
    manual_v = data.manual_v
    stop = data.stop
    command_v = data.command_v

X = 0.0 # Global X
Y = 0.0 # Global Y
yaw = 0.0 # Global yaw
h = 8.0
vl = 0.0
u = 0.0
y = 0.0
v = 0.0
phi = 0.0
r = 0.0
rd = 0.0
prev_i = 0.0
prev_s = 0.0
servoRatio = 8.9479e-04
SERVOMID = 1537
LANEWIDTH = 0.3
VMAX = 0.5

manual_steer = True
right_lane = True
manual_v = True
stop = False
command_v = 0.0

rospy.init_node('Central_control2')
motor_pub = rospy.Publisher("Motor_command2", Float32, queue_size=10)
servo_pub = rospy.Publisher("servo2", UInt16, queue_size=10)
rospy.Subscriber("CarState2", CarState, CarState_callback)
rospy.Subscriber("Control_command2", CentralSignal, Control_callback)

print "Servo ratio: ", servoRatio

rate = rospy.Rate(10.0)
motorMsg = Float32()
motorMsg.data = 0.0
servoMsg = UInt16()
servoMsg.data = SERVOMID

while not rospy.is_shutdown():

    if stop:
        motorMsg.data = 0.0
        prev_i = 0.0
    else:
        if manual_v: # CC mode based on the velocity set by manual gui
            # print "command_v", command_v
            motorMsg.data = 0.0
            if np.abs(command_v) > 0.01:
                rospy.wait_for_service('MPC_CC2', 0.1)
                try:
                    MPC_CC_compute = rospy.ServiceProxy('MPC_CC2', MPC_CC)
                    vr, u0, i0 = command_v, u, prev_i
                    wv, wi = 10000, 400
                    i_max, i_min = 4.5, -3
                    resp = MPC_CC_compute(vr, u0, i0, wv, wi, i_max, i_min)
                    print "Computed i: ", resp.i
                    motorMsg.data = resp.i
                except rospy.ServiceException, e:
                        print "Service call failed for CC: %s"%e
                        motorMsg.data = 0.0
        else:
            # ACC + CC
            motorMsg.data = 0.0
            if h > 7.5: # then travel with a speed of VMAX
                rospy.wait_for_service('MPC_CC2', 0.1)
                try:
                    MPC_CC_compute = rospy.ServiceProxy('MPC_CC2', MPC_CC)
                    vr, u0, i0 = VMAX, u, prev_i
                    wv, wi = 10000, 400
                    i_max, i_min = 4.5, -3
                    resp = MPC_CC_compute(vr, u0, i0, wv, wi, i_max, i_min)
                    print "Computed i: ", resp.i
                    motorMsg.data = resp.i
                except rospy.ServiceException, e:
                        print "Service call failed for CC: %s"%e
                        motorMsg.data = 0.0
            else:
                rospy.wait_for_service('MPC_ACC2', 0.1)
                try:
                    MPC_ACC_compute = rospy.ServiceProxy('MPC_ACC2', MPC_ACC)
                    u0, h0, i0 = u, h, prev_i
                    wv, wh, wi = 10, 100, 5
                    h_stop, T_gap = 0.3, 0.5
                    v_max, v_min, h_min = 2, -0.1, 0.3
                    i_max, i_min = 4.5, -3
                    resp = MPC_ACC_compute(u0,h0,vl,prev_i,wv,wh,wi,h_stop,T_gap,v_max,v_min,h_min,i_max,i_min)
                    print "Computed i: ", resp.i
                    motorMsg.data = resp.i
                except rospy.ServiceException, e:
                        print "Service call failed for ACC: %s"%e
                        motorMsg.data = 0.0

    motor_pub.publish(motorMsg)

    if not manual_steer:
        if u > 0.1:
            print "start LK"
            rospy.wait_for_service('MPC_LK2', 0.1)
            try:
                MPC_LK_compute = rospy.ServiceProxy('MPC_LK2', MPC_LK)
                if right_lane:
                    y0 = y - LANEWIDTH
                else:
                    y0 = y + LANEWIDTH
                v0,p0,r0,s0 = v,phi,r,prev_s
                wy,wv,wp,wr,ws = 100.0, 10.0, 50.0, 30.0, 10.0
                y_max,y_min,v_max,p_max,r_max,s_max = 0.8,-0.8,0.5,50.0/180.0*np.pi,20.0/180.0*np.pi,15.0/180.0*np.pi
                A,B,E = getLatDynMat(u)
                rd = tuple(np.zeros((21,), dtype = np.float64))
                resp = MPC_LK_compute(y0,v0,p0,r0,s0,wy,wv,wp,wr,ws,y_max,y_min,v_max,p_max,r_max,s_max,A,B,E,rd)
                servoMsg.data = SERVOMID + int(resp.s/servoRatio)
                print "Computed steer: ", int(resp.s/servoRatio)
                servo_pub.publish(servoMsg)
            except rospy.ServiceException, e:
                    print "Service call failed for LK: %s"%e
            


    # print "manual_steer", manual_steer
    # print "right_lane", right_lane
    # print "manual_v", manual_v
    # print "stop", stop
    # print "command_v", command_v
    # print "-------------------"

    rate.sleep()