#include "ros/ros.h"
#include "std_msgs/UInt16.h"
#include "std_msgs/Float32.h"
#include "std_msgs/String.h"
#include "Car_parameters.h"
#include <um_acc/Motor_data.h>

#include <sstream>
#include <iostream>
#include <string>

bool working = false;
double v_now = 0;
double v_prev = 0;
double v_int = 0;
double t_prev = 0;
double t_begin = 0;
double t_now = 0;
double v_ref = 0;
double accel = 0;
double KI = 0;
double KP = 3.5;
double KD = 0.5;

ros::Publisher motor_pub;
ros::Subscriber motor_sub;
ros::Subscriber control_sub;
std_msgs::Float32 motorMsg;


void motorCallback(const um_acc::Motor_data::ConstPtr& msg)
{
  std::cout << "Motor callback" << std::endl;
  v_prev = v_now;
  v_now = msg->countPerSecond * CPS2V;
  t_prev = t_now;
  t_now = ros::Time::now().toSec() - t_begin;
  //std::cout << t_now << " ;" << t_prev << " ;" << t_now-t_prev << std::endl;
  accel = (v_now - v_prev)/(t_now - t_prev);
  v_int += (v_ref - v_now)*(t_now - t_prev);
  std::cout << "velocity: "<< v_now << " accel: " << accel << std::endl;
}

void controlCallback(const std_msgs::String::ConstPtr& msg)
{
  std::stringstream ss;
  ss << msg->data;
  std::string mode;
  ss >> mode;
  std::cout << mode << std::endl;
  std::cout << "here" << std::endl;
  if (mode == "CC"){
    ss >> v_ref;
    working = true;
  }
  else{
    v_ref = 0;
    working = false;
    motorMsg.data = 0;
    motor_pub.publish(motorMsg);
    ros::spinOnce();
  }
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "CC_simple_node");

  ros::NodeHandle n;

  motor_pub = n.advertise<std_msgs::Float32>("Motor_command", 1);
  motor_sub = n.subscribe("Motor_data", 1, motorCallback);
  control_sub = n.subscribe("Control_interface", 1, controlCallback);

  ros::Rate loop_rate(25);
  t_begin = ros::Time::now().toSec();

  std::cout << "start" << std::endl;

  while (ros::ok())
  {

    while (working){
      motorMsg.data = KI * v_int + KP * (v_ref - v_now) - KD * accel;
      std::cout << motorMsg.data << std::endl;

      if (motorMsg.data > MAXCURRENT){
        motorMsg.data = MAXCURRENT;
      }
      else if (motorMsg.data < -MAXCURRENT){
        motorMsg.data = -MAXCURRENT;
      }

      ROS_INFO("Current: %f, velocity: %f", motorMsg.data, v_now);
      motor_pub.publish(motorMsg);

      ros::spinOnce();
      loop_rate.sleep();
    }
    //motorMsg.data = 0;
    //motor_pub.publish(motorMsg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  std::cout << "Stopped" << std::endl;
  return 0;
}
