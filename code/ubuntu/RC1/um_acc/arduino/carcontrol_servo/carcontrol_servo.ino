#include <Servo.h>
#include <ros.h>
#include <um_acc/Pwm.h>

int esc_pin = 9;
int servo_pin = 10;

Servo esc, servo;
ros::NodeHandle  nh;

// Subscriber to topic
void vel_cb(const um_acc::Pwm& cmd_msg) {
  esc.writeMicroseconds(cmd_msg.vel_pwm);
  servo.writeMicroseconds(cmd_msg.steer_pwm);
}
ros::Subscriber<um_acc::Pwm> sub_pwm("pwm_control", &vel_cb );

void setup() {
  nh.initNode();
  nh.subscribe(sub_pwm);
  
  esc.attach(esc_pin);
  servo.attach(servo_pin);
  esc.writeMicroseconds(1500);
  servo.writeMicroseconds(1500);
}

void loop() {
  nh.spinOnce();
  delay(1);  
}
