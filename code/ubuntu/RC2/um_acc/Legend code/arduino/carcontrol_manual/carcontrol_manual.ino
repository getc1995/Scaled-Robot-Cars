#include <ros.h>
#include <um_acc/Pwm.h>

#include <pwm01.h>

ros::NodeHandle  nh;

int vel_pin = 9;
int steer_pin = 10;
uint16_t pwm_freq = 100;     // desired frequency
uint16_t max_duty = -1;      // = 65535
double actual_period = 9960; // in microseconds
uint16_t normal_duty = 9830;

// Subscriber to topic
void vel_cb(const um_acc::Pwm& cmd_msg) {
  double vel_fra = cmd_msg.vel_pwm/actual_period;
  uint16_t vel_duty = vel_fra*max_duty;
  pwm_write_duty(vel_pin, vel_duty);

  double steer_fra = cmd_msg.steer_pwm/actual_period;
  uint16_t steer_duty = steer_fra*max_duty;
  pwm_write_duty(steer_pin, steer_duty);
}
ros::Subscriber<um_acc::Pwm> sub_pwm("pwm_control", &vel_cb );

void setup() {
  pwm_set_resolution(16);
  pwm_setup(vel_pin, pwm_freq, 2); // Must specify clock
  pwm_setup(steer_pin, pwm_freq, 2); // Must specify clock
  
  pwm_write_duty(vel_pin, normal_duty);
  pwm_write_duty(steer_pin, normal_duty);

  nh.initNode();
  nh.subscribe(sub_pwm);
}

void loop() {
  nh.spinOnce();
  delay(1);  
}
