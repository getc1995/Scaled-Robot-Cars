#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <unistd.h>
#include "rs232.h"
#include "ros/ros.h"
#include <um_acc/Motor_data.h>
#include <um_acc/Lidar_data.h>
#include <std_msgs/Float32.h>

#define DELAY 10000

using namespace std;

int i=0,
    cport_nr=0,		// /dev/ELMO		defined in rs232.cpp
    bdrate=115200;	/* 115200 baud */

char mode[]={'8','N','1',0};

char currentStr[50] = "TC=0.0\r";
um_acc::Motor_data msg;

void motorCallback(const std_msgs::Float32::ConstPtr& Rmsg)
{
	sprintf(currentStr, "TC=%0.3lf\r", -Rmsg->data);
	cout << currentStr << endl;
}

void lidarCallback(const um_acc::Lidar_data::ConstPtr& Rmsg)
{
	msg.h = Rmsg->h;
	msg.hdot = Rmsg->hdot;
}

/*inline void getParameter(string & str, um_acc::Motor_data & msg){
	char c = str[0];
	str.erase(0, 3);
	stringstream ss;
	ss << str;
	switch (c){
		case 'V':  ss >> msg.countPerSecond;
		break;

		case 'P':  ss >> msg.position;
		break;

		case 'I':  ss >> msg.current;
		break;

		default: break;
	}
}*/

int main(int argc, char **argv){
	ros::init(argc, argv, "motor_node2");
	ros::NodeHandle nh;
	ros::Publisher motor_pub = nh.advertise<um_acc::Motor_data>("Motor_data2", 1000);
	ros::Subscriber motor_sub = nh.subscribe("Motor_command2", 1, motorCallback);
	ros::Subscriber lidar_sub = nh.subscribe("Lidar_data2", 1, lidarCallback);

	ros::Rate loop_rate(80);
	// double now = 0, prev = 0;

    msg.countPerSecond = 0;
    msg.position = 0;
    msg.current = 0;
    msg.h = 0;
    msg.hdot = 0;

	cout << "start" << endl;
	
	if(RS232_OpenComport(cport_nr, bdrate, mode))
	{
		printf("Can not open comport\n");
		return(0);
	}
	else
	{
		printf("comport open\n");
	}

	unsigned char buf[128];
	stringstream ss;
	ss << std::setprecision(3) << std::fixed;
	bool notUpdated = false;
	string str, junk;

	str = "MO=1\r";
	RS232_cputs(cport_nr, str.c_str());
	int n = RS232_PollComport(cport_nr, buf, 127);
	usleep(DELAY);

	str = "UM=1\r";
	RS232_cputs(cport_nr, str.c_str());
	n = RS232_PollComport(cport_nr, buf, 127);
	usleep(DELAY);

	while(ros::ok()){
		/*ss << "TC=" << current << '\r';
		ss >> str;
		ss.clear();
        ss.str("");*/
        //str = "TC=0.2\r";
		RS232_cputs(cport_nr, currentStr);
		n = RS232_PollComport(cport_nr, buf, 127);
		if(n > 0){
			buf[n-1]='\0';
			buf[2]=' ';
			ss << buf;
			switch (buf[0]){
				case 'V':  ss >> junk >> msg.countPerSecond;
				msg.countPerSecond = -msg.countPerSecond;
				break;

				case 'P':  ss >> junk >> msg.position;
				msg.position = -msg.position;
				break;

				case 'I':  ss >> junk >> msg.current;
				msg.current = -msg.current;
				break;

				default: break;
			}
			ss.clear();
	        ss.str("");
		}
		else{
			cout << "Notreceived" << endl;
			notUpdated = true;
		}
		loop_rate.sleep();
		//usleep(DELAY);
		

        /*Position*/
		str = "PX\r";
		RS232_cputs(cport_nr, str.c_str());
		n = RS232_PollComport(cport_nr, buf, 127);
		if(n > 0){
			buf[n-1]='\0';
			buf[2]=' ';
			ss << buf;
			switch (buf[0]){
				case 'V':  ss >> junk >> msg.countPerSecond;
				msg.countPerSecond = -msg.countPerSecond;
				break;

				case 'P':  ss >> junk >> msg.position;
				msg.position = -msg.position;
				break;

				case 'I':  ss >> junk >> msg.current;
				msg.current = -msg.current;
				break;

				default: break;
			}
			ss.clear();
	        ss.str("");
		}
		else{
			cout << "Notreceived" << endl;
			notUpdated = true;
		}
		loop_rate.sleep();
		//usleep(DELAY);

		/*velocity*/
		str = "VX\r";
		RS232_cputs(cport_nr, str.c_str());
		n = RS232_PollComport(cport_nr, buf, 127);
		if(n > 0){
			buf[n-1]='\0';
			buf[2]=' ';
			ss << buf;
			switch (buf[0]){
				case 'V':  ss >> junk >> msg.countPerSecond;
				msg.countPerSecond = -msg.countPerSecond;
				break;

				case 'P':  ss >> junk >> msg.position;
				msg.position = -msg.position;
				break;

				case 'I':  ss >> junk >> msg.current;
				msg.current = -msg.current;
				break;

				default: break;
			}
			ss.clear();
	        ss.str("");
		}
		else{
			cout << "Notreceived" << endl;
			notUpdated = true;
		}
		loop_rate.sleep();
		//usleep(DELAY);
		

		/*Current*/
		str = "IQ\r";
		RS232_cputs(cport_nr, str.c_str());
		n = RS232_PollComport(cport_nr, buf, 127);
		if(n > 0){
			buf[n-1]='\0';
			buf[2]=' ';
			ss << buf;
			switch (buf[0]){
				case 'V':  ss >> junk >> msg.countPerSecond;
				msg.countPerSecond = -msg.countPerSecond;
				break;

				case 'P':  ss >> junk >> msg.position;
				msg.position = -msg.position;
				break;

				case 'I':  ss >> junk >> msg.current;
				msg.current = -msg.current;
				break;

				default: break;
			}
			ss.clear();
	        ss.str("");
		}
		else{
			cout << "Notreceived" << endl;
			notUpdated = true;
		}
		loop_rate.sleep();
		//usleep(DELAY);

		/*Publish data*/
    	motor_pub.publish(msg);

			
		ros::spinOnce();
		notUpdated = false;
		// now = ros::Time::now().toSec();
  //   	std::cout << now - prev << std::endl;
  //   	prev = now;
		
	}

	return 0;
}