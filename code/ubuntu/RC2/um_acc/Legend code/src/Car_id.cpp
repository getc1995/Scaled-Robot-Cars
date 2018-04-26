#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include "ros/ros.h"
#include <um_acc/Pwm.h>
#include <um_acc/Sensor.h>
#include "SimpleGPIO.h"
#include "SimplePWM.h"
#include <pthread.h>
#include "rs232.h"

using namespace std;

int i=0,
    cport_nr=1,		/* /dev/ttyACM1 */
    bdrate=115200;	/* 115200 baud */

char mode[]={'8','N','1',0};
ostringstream os;

unsigned int PING_GPIO = 161;
unsigned int SW_GPIO = 164;
unsigned int STEER_GPIO = 163;
unsigned int STEER_PIN = 2;
unsigned int DRIVE_PIN = 0;

float microsecondsToCentimeters(float microseconds)
{
    return microseconds / 29 / 2;
};
 
void *Sonar(void *threadid)
{
    float duration, inches, cm;
    ros::NodeHandle nsonar;
    ros::Publisher pub = nsonar.advertise<um_acc::Sensor>("sensor", 1000);
 
    um_acc::Sensor sen;
 
    while(1)
    {
        gpio_export(PING_GPIO);
        gpio_set_dir(PING_GPIO, OUTPUT_PIN);
        gpio_set_value(PING_GPIO, LOW);
        usleep(2);
        gpio_set_value(PING_GPIO, HIGH);
        usleep(5);
        gpio_set_value(PING_GPIO, LOW);
 
        gpio_set_dir(PING_GPIO, INPUT_PIN);
  
        duration = gpio_pulse_in(PING_GPIO, HIGH);
 
        cm = microsecondsToCentimeters(duration);
cout << cm << endl;
        sen.distance = cm;
 
        //do something to get rpm from Elmo
 
        sen.rpm = 5;
 
        pub.publish(sen);
 
        usleep(100000);
    }
};
 
void pwmCallback(const um_acc::Pwm::ConstPtr& msg)
{
    pwm_set_duty(STEER_PIN, msg->steer_pwm*1000);
    pwm_set_duty(DRIVE_PIN, msg->vel_pwm*1000);
};
 
void *Car(void *threadid)
{

	gpio_export(STEER_GPIO);
	gpio_set_dir(STEER_GPIO, OUTPUT_PIN);
	
	while(1)
	{		
		gpio_set_value(STEER_GPIO, LOW);
		usleep(5);
		gpio_set_value(STEER_GPIO, HIGH);
		usleep(3);
	}
	ros::spin();

/*
    pwm_export(STEER_PIN);
    pwm_set_period(STEER_PIN, 5000000);
    pwm_set_duty(STEER_PIN, 1600000);
    pwm_enable(STEER_PIN);
 
    pwm_export(DRIVE_PIN);
    pwm_set_period(DRIVE_PIN, 5000000);
    pwm_set_duty(DRIVE_PIN, 1600000);
    pwm_enable(DRIVE_PIN);
 
    ros::NodeHandle ncar;
    ros::Subscriber sub = ncar.subscribe("pwm_control", 1000, pwmCallback);
 
    ros::spin();
*/
};


void *printScreen(void *threadid){
	RS232_flushRX(cport_nr);
	int n = 0;
	cout << "Print start:" << endl;
	unsigned char buf[2];
	while(ros::ok()){
		n = RS232_PollComport(cport_nr, buf, 1);
		//cout << (char *)buf;
		if(n > 0){
			//cout << "Anything" << endl;
			if (buf[0] == ';'){
				cout << os.str() << endl;
				os.str("");
				os.clear();
			}
			else{
				if (buf[0] == '\r')
					os << "~\n";
				else
					os << (char)buf[0];
			}

/**			if (buf[0] == ';'){
				os << ";\n";
				//os.str("");
				//os.clear();
			}
			else{
				if (buf[0] == '\r')
					os << "~\n";
				else
					os << buf[0];
			}**/
		}
	}
}

void *action(void *threadid){
	unsigned char REC[100];

	cout << "Done" << endl;

}

int main(int argc, char *argv[]){
 	//ios_base::sync_with_stdio(false);
 	ros::init(argc, argv, "basic_node");
	cout << "start" << endl;
	//ros::init(argc, argv, "listener");
	//pthread_t threads[2];
	//pthread_create(&threads[0], NULL, printScreen, NULL);
	//pthread_create(&threads[1], NULL, action, NULL);
	//pthread_create(&threads[0], NULL, Sonar, NULL);
	//pthread_create(&threads[1], NULL, Car, NULL);

	unsigned char REC[100] = "\0";

	if(RS232_OpenComport(cport_nr, bdrate, mode))
	{
		printf("Can not open comport\n");
		return(0);
	}
	else
	{
		printf("comport open\n");
	}

	//pthread_join(threads[0], NULL);
	//pthread_join(threads[1], NULL);
	
	RS232_flushTX(cport_nr);
	cout << endl;


	unsigned char buf[101];
	string str = "\rMO=1\r";
	RS232_cputs(cport_nr, str.c_str());
	int n = RS232_PollComport(cport_nr, buf, 100);
	if(n > 0){
		buf[n]='\0';
		cout << buf << endl;
	}
	usleep(100);

	str = "\rUM=1\r";
	RS232_cputs(cport_nr, str.c_str());
	n = RS232_PollComport(cport_nr, buf, 100);
	if(n > 0){
		buf[n]='\0';
		cout << buf << endl;
	}
	usleep(20000);
while(ros::ok())
	{
		unsigned char buf[101];
		
		
		/**str = "\rPX\r";
		RS232_cputs(cport_nr, str.c_str());
		n = RS232_PollComport(cport_nr, buf, 100);
		if(n > 0){
			buf[n]='\0';
			cout << buf << endl;
		}
		//usleep(10);**/

		string str = "TC=0.2\r";
		RS232_cputs(cport_nr, str.c_str());
		n = RS232_PollComport(cport_nr, buf, 100);
		if(n > 0){
			buf[n]='\0';
			cout << buf << endl;
		}
		//usleep(10);

		usleep(1000);
		
		/**str = "\rVX\r";
		RS232_cputs(cport_nr, str.c_str());
		usleep(10);**/
		
		str = "TC=-0.2\r";
		RS232_cputs(cport_nr, str.c_str());
		n = RS232_PollComport(cport_nr, buf, 100);
		if(n > 0){
			buf[n]='\0';
			cout << buf << endl;
		}
		//usleep(10);
		usleep(1000);
		
		str = "VX\r";
		RS232_cputs(cport_nr, str.c_str());
		n = RS232_PollComport(cport_nr, buf, 100);
		if(n > 0){
			buf[n]='\0';
			cout << buf << endl;
		}
		usleep(1000);

		str = "PX\r";
		RS232_cputs(cport_nr, str.c_str());
		n = RS232_PollComport(cport_nr, buf, 100);
		if(n > 0){
			buf[n]='\0';
			cout << buf << endl;
		}
		usleep(1000);

		str = "IQ\r";
		RS232_cputs(cport_nr, str.c_str());
		n = RS232_PollComport(cport_nr, buf, 100);
		if(n > 0){
			buf[n]='\0';
			cout << buf << endl;
		}
		usleep(1000);

		/**usleep(2500000);
		
		str = "\rTC=0\r";
		RS232_cputs(cport_nr, str.c_str());
		usleep(10);
		
		str = "\rIQ\r";
		RS232_cputs(cport_nr, str.c_str());
		usleep(10);

		str = "\rVX\r";
		RS232_cputs(cport_nr, str.c_str());
		usleep(10);

		str = "\rPX\r";
		RS232_cputs(cport_nr, str.c_str());
		usleep(10);

		str = "\rMO=0\r";
		RS232_cputs(cport_nr, str.c_str());
		usleep(10);**/

	}

	string endstr = "\rTC=0\r";
	RS232_cputs(cport_nr, endstr.c_str());
	usleep(2000000);

	endstr = "\rMO=0\r";
	RS232_cputs(cport_nr, endstr.c_str());
	usleep(2000000);


	//cout << RS232_PollComport(cport_nr, REC, 100) << endl;
	//cout << (char *)REC << endl;

	RS232_CloseComport(cport_nr);
 	return(0);
}
