# PWM RC controller
Arduino sketch that outputs 100 Hz PWM signals at pins 8 and 9. 

## Dependencies
* `pwm01` package from https://github.com/it-workshop/pwm01.h_by_randomvibe

## Create ros_lib
Make sure the ROS package `um_acc` is compiled.
```
  source (catkin_ws)/devel/setup.zsh
  rosrun rosserial_arduino make_libraries.py .
```

## Compile sketch
Open `setpwm.ino` in the Arduino IDE, compile it and upload it to the server.

## Connect to arduino from ROS
Will create a subscriber to the topic "pwm_control"
```
   rosrun rosserial_python serial_node.py _port:=/dev/ttymxc3
```