  /*
 * rosserial Servo Control Example
 *
 * This sketch demonstrates the control of hobby R/C servos
 * using ROS and the arduiono
 * 
 * For the full tutorial write up, visit
 * www.ros.org/wiki/rosserial_arduino_demos
 *
 * For more information on the Arduino Servo Library
 * Checkout :
 * http://www.arduino.cc/en/Reference/Servo
 */

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include <Servo.h> 
#include <ros.h>
#include <std_msgs/UInt16.h>
#include <Wire.h>
//#include <LIDARLite.h>

//LIDARLite myLidarLite;

ros::NodeHandle  nh;

Servo servo;

std_msgs::UInt16 Distance;  
//std_msgs::UInt16 Servo_pwm;

void servo_cb( const std_msgs::UInt16& cmd_msg){
  servo.writeMicroseconds(cmd_msg.data);
//  Servo_pwm.data = cmd_msg.data;
}

ros::Subscriber<std_msgs::UInt16> sub("servo2", servo_cb);
//ros::Publisher lidar("lidar", &Distance);
//ros::Publisher servo_fb("servo_fb", &Servo_pwm);

void setup(){
  pinMode(13, OUTPUT);
  digitalWrite(13,HIGH);

  nh.initNode();
  nh.subscribe(sub);
//  nh.advertise(lidar);
  //nh.advertise(servo_fb);
  
  servo.attach(9); //attach it to pin 9
//  Serial.begin(115200);
//  myLidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz
//  myLidarLite.configure(0); // Change this number to try out alternate configurations
  servo.writeMicroseconds(1537); //Set the initial servo angular position to the middle.
}

void loop(){
//  Distance.data = myLidarLite.distance();
//  lidar.publish( &Distance );
//  servo_fb.publish(&Servo_pwm);
  nh.spinOnce();
  //delay(1);
//  Serial.println(Distance.data);
}
