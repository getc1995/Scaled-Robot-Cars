#include <ros/ros.h>
#include <signal.h>
#include <termios.h>
#include <stdio.h>
#include "std_msgs/UInt16.h"
#include "std_msgs/Float32.h"

#define max(a,b)   ((a > b)? a : b)
#define min(a,b)   ((a < b)? a : b)

#define KEYCODE_R 0x43 
#define KEYCODE_L 0x44
#define KEYCODE_U 0x41
#define KEYCODE_D 0x42
#define KEYCODE_Q 0x71

#define CUR_STEP 0.1
#define STEER_STEP 1

ros::Publisher servo_pub;
ros::Publisher motor_pub;

class TeleopCar
{
public:
  TeleopCar();
  void keyLoop();

private:
 
  unsigned int steer;
  float current;
  std_msgs::UInt16 servoMsg;
  std_msgs::Float32 motorMsg;  
};

TeleopCar::TeleopCar()
{
  steer = 1414;
  current = 0;
}

int kfd = 0;
struct termios cooked, raw;

void quit(int sig)
{
  tcsetattr(kfd, TCSANOW, &cooked);
  ros::shutdown();
  exit(0);
}


int main(int argc, char** argv)
{
  ros::init(argc, argv, "remote_car");
  ros::NodeHandle nh;
  servo_pub = nh.advertise<std_msgs::UInt16>("servo", 1);
  motor_pub = nh.advertise<std_msgs::Float32>("Motor_command", 1);
  ros::Rate loop_rate(100);
  TeleopCar teleop_car;

  signal(SIGINT,quit);

  teleop_car.keyLoop();
  
  return(0);
}


void TeleopCar::keyLoop()
{
  char c;
  bool steerDirty=false;
  bool motorDirty=false;

  tcgetattr(kfd, &cooked);
  memcpy(&raw, &cooked, sizeof(struct termios));
  raw.c_lflag &=~ (ICANON | ECHO);
  raw.c_cc[VEOL] = 1;
  raw.c_cc[VEOF] = 2;
  tcsetattr(kfd, TCSANOW, &raw);

  puts("Reading from keyboard");
  puts("---------------------------");
  puts("Use arrow keys to move the car.");


  for(;;) {
      if(read(kfd, &c, 1) < 0) {
    	  perror("read():");
    	  exit(-1);
      }

      ROS_DEBUG("value: 0x%02X\n", c);
  
      switch(c)	{
      	case KEYCODE_L:
      	  ROS_DEBUG("LEFT");
      	  steer = max(1200, steer-STEER_STEP);
      	  steerDirty = true;
      	  break;
      	case KEYCODE_R:
      	  ROS_DEBUG("RIGHT");
      	  steer = min(1700, steer+STEER_STEP);
      	  steerDirty = true;
      	  break;
      	case KEYCODE_U:
      	  ROS_DEBUG("UP");
          current = max(2.7, current);
      	  current = min(4, current+CUR_STEP);
      	  motorDirty = true;
      	  break;
      	case KEYCODE_D:
      	  ROS_DEBUG("DOWN");
          current = min(-2.7, current);
      	  current = max(-4, current-CUR_STEP);
      	  motorDirty = true;
      	  break;

        case ' ':
          ROS_DEBUG("SPACE");
          current = 0;
          motorDirty = false;
          break;

        default: break;
      }

      if (motorDirty) {
        motorMsg.data = current;
      	motor_pub.publish(motorMsg);
        std::cout << "Current:" << current << std::endl;
      }
      else{
        current = 0;
        motorMsg.data = current;
        motor_pub.publish(motorMsg);
        std::cout << "Current:" << "zero" << std::endl;
      }

      if (steerDirty) {
        servoMsg.data = steer;
        servo_pub.publish(servoMsg);
        std::cout << "Servo: " << steer << std::endl;
      }
      ros::spinOnce();
      ros::Rate loop_rate(100);
    }

  return;
}
