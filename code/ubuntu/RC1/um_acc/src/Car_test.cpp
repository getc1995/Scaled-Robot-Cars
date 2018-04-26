#include "ros/ros.h"
#include "std_msgs/UInt16.h"
#include "std_msgs/Float32.h"

#include <sstream>
#include <iostream>


int main(int argc, char **argv)
{

  ros::init(argc, argv, "Car_test_talker");

  ros::NodeHandle n;

  ros::Publisher servo_pub = n.advertise<std_msgs::UInt16>("servo", 1000);
  ros::Publisher motor_pub = n.advertise<std_msgs::Float32>("Motor_command", 1000);

  ros::Rate loop_rate(0.5);

  std_msgs::UInt16 servoMsg;
  std_msgs::Float32 motorMsg;

  while (ros::ok())
  {

    /*
    servoMsg.data = 1500;
    motorMsg.data = 0.2;

    ROS_INFO("Steering: %d, current: %f\n", servoMsg.data, motorMsg.data);
    servo_pub.publish(servoMsg);
    motor_pub.publish(motorMsg);

    ros::spinOnce();
    loop_rate.sleep();

//
    servoMsg.data = 1600;
    motorMsg.data = -0.2;

    ROS_INFO("Steering: %d, current: %f\n", servoMsg.data, motorMsg.data);
    servo_pub.publish(servoMsg);
    motor_pub.publish(motorMsg);

    ros::spinOnce();
    loop_rate.sleep();*/

//
    servoMsg.data = 1800;
    motorMsg.data = 0;

    ROS_INFO("Steering: %d, current: %f\n", servoMsg.data, motorMsg.data);
    servo_pub.publish(servoMsg);
    motor_pub.publish(motorMsg);

    ros::spinOnce();
    loop_rate.sleep();
   
  }

  std::cout << "Stopped" << std::endl;
  return 0;
}
