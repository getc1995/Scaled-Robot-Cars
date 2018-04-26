#include <um_acc/IMU.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include "ros/ros.h"
#include <um_acc/Motor_data.h>
#include <std_msgs/Float32.h>
#include "std_msgs/UInt16.h"
#include "std_msgs/Float32.h"

using namespace std;

um_acc::Motor_data motorMsg;
um_acc::IMU IMUMsg;
std_msgs::UInt16 servoMsg;
std_msgs::Float32 currentMsg;


void IMUCallback(const um_acc::IMU::ConstPtr& msg)
{
  IMUMsg.time = msg->time;
  IMUMsg.ax = msg->ax;
  IMUMsg.ay = msg->ay;
  IMUMsg.az = msg->az;
  IMUMsg.gx = msg->gx;
  IMUMsg.gy = msg->gy;
  IMUMsg.gz = msg->gz;
  IMUMsg.mx = msg->mx;
  IMUMsg.my = msg->my;
  IMUMsg.mz = msg->mz;
}

void motorCallback(const um_acc::Motor_data::ConstPtr& msg)
{
  motorMsg.current = msg->current;
  motorMsg.position = msg->position;
  motorMsg.countPerSecond = msg->countPerSecond;
}

void servoCallback(const std_msgs::UInt16::ConstPtr& msg)
{
  servoMsg.data = msg->data;
}

void mcommandCallback(const std_msgs::Float32::ConstPtr& msg)
{
  currentMsg.data = msg->data;
}

int main(int argc, char *argv[]){
	ofstream myfile;
	myfile.open("/media/ubuntu/SD/data.txt");

	ros::init(argc, argv, "Record_node");
	ros::NodeHandle n;
	ros::Rate loop_rate(25);

	ros::Subscriber IMU_sub = n.subscribe("IMU_data", 2, IMUCallback);
	ros::Subscriber motor_sub = n.subscribe("Motor_data", 2, motorCallback);
	ros::Subscriber servo_sub = n.subscribe("servo", 2, servoCallback);
	ros::Subscriber mcommand_sub = n.subscribe("Motor_command", 2, mcommandCallback);
	servoMsg.data = 0;

	time_t t = time(0);
	while(ros::ok()){
		//t = time(0);
		//char* dt = ctime(&t);

		myfile << currentMsg.data << ' ' << servoMsg.data 
		<< ' ' << motorMsg.current 
		<< ' ' << motorMsg.position
		<< ' ' << motorMsg.countPerSecond
		<< " "<< IMUMsg.ax 
		<< " "<< IMUMsg.ay 
		<< " "<< IMUMsg.az 
		<< " "<< IMUMsg.gx 
		<< " "<< IMUMsg.gy 
		<< " "<< IMUMsg.gz 
		<< " "<< IMUMsg.mx
		<< " "<< IMUMsg.my
		<< " "<< IMUMsg.mz
		<< " "<< endl;
		ros::spinOnce();
    	loop_rate.sleep();
	}
	return 0;
}