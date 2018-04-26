#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions
#include <um_acc/IMU.h>
#include <iostream>
#include <sstream>
#include <eigen3/Eigen/Dense>
#include <vector>
#include "ros/ros.h"

using namespace Eigen;

class butterworth{
private:
	//A second order butterworth filter for IMU only with a cornering frequency of 20Hz and a sampling frequency of 100hz.
	Matrix2d A;
	Matrix<double, 2, 1> B;
	Matrix<double, 1, 2> C;
	double D;
	Vector2d x;
public:
	butterworth(){
		x << 0, 0;
		A << -0.217328454996463, -0.568644162904732,
			 0.568644162904732,	  0.586855832347704;
		B << 0.804184287344167,
			 0.584274085109220;
		C << 0.201046071836042,	0.561038259909243;
		D = 0.206572083826148;
	}
	double filt(double input){
		x = A*x + B*input;
		return C*x + D*input;
	}
};


int main(int argc, char *argv[]){
	/* Open File Descriptor */
	int USB = open( "/dev/IMU", O_RDWR| O_NONBLOCK | O_NDELAY );

	/* Error Handling */
	if ( USB < 0 )
	{
		std::cout << "Error " << errno << " opening " << "/dev/ttyACM1" << ": " << strerror (errno) << std::endl;
	}


	struct termios tty;
	struct termios tty_old;
	memset (&tty, 0, sizeof tty);

	/* Error Handling */
	if ( tcgetattr ( USB, &tty ) != 0 ) {
	   std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
	}

	/* Save old tty parameters */
	tty_old = tty;

	/* Set Baud Rate */
	cfsetospeed (&tty, (speed_t)B115200);
	cfsetispeed (&tty, (speed_t)B115200);

	/* Setting other Port Stuff */
	tty.c_cflag     &=  ~PARENB;            // Make 8n1
	tty.c_cflag     &=  ~CSTOPB;
	tty.c_cflag     &=  ~CSIZE;
	tty.c_cflag     |=  CS8;

	tty.c_cflag     &=  ~CRTSCTS;           // no flow control
	tty.c_cc[VMIN]   =  1;                  // read doesn't block
	tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
	tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

	/* Make raw */
	cfmakeraw(&tty);

	/* Flush Port, then applies attributes */
	tcflush( USB, TCIFLUSH );
	if ( tcsetattr ( USB, TCSANOW, &tty ) != 0) {
	   std::cout << "Error " << errno << " from tcsetattr" << std::endl;
	}


	/*Start the loop*/
	ros::init(argc, argv, "IMU_talker1");
	ros::NodeHandle n;
    ros::Publisher IMU_pub = n.advertise<um_acc::IMU>("IMU_data1", 1000);
    um_acc::IMU msg;

    butterworth yaw_rate_filter;

    std::stringstream ss;
    bool init = false;
	std::vector<double> initVec;
	int size = 1024;
	initVec.reserve(size);
	double median = 0;

	while(ros::ok()){

		int n = 0;
		int spot = 0;
		char buf = '\0';

		/* Whole response*/
		char response[1024];
		memset(response, '\0', sizeof response);

		do {
		    n = read( USB, &buf, 1 );
		    sprintf( &response[spot], "%c", buf );
		    spot += n;
		} while( buf != '\n' && n > 0);

		
		if (n > 0) {
		    std::cout << "Response: " << response << std::endl;
		    ss << response;
		    ss >> msg.time >> msg.ax >> msg.ay >> msg.az >> msg.gx >> msg.gy >> msg.gz >> msg.mx >> msg.my >> msg.mz;
		    //ROS_INFO("%d", msg.time);
		    msg.gz = yaw_rate_filter.filt(msg.gz) - median;
		    if (!init){
		    	initVec.push_back(msg.gz);
		    	if (initVec.size()>size-1){
		    		median = 0;
		    		for (int i = 0; i < size; i++){
		    			median += initVec[i];
		    		}
		    		median = median/double(initVec.size());
		    		init = true;
		    	}
		    	msg.gz = 0.0;
		    }

		    IMU_pub.publish(msg);
        	ros::spinOnce();
        	ss.clear();
            ss.str("");
            usleep(8000);//The IMU is running on a period of 10000, so it's safe to sleep for 8000.
		}
	}
	return 0;
}