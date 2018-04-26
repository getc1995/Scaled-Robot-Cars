#!/bin/bash

echo "Setup"
stty -F /dev/ttyACM0 115200
stty -F /dev/ttyACM1 115200
stty -F /dev/ttyACM2 115200
source rccar_catkin_ws/devel/setup.bash

rosnode kill amcl
rosnode cleanup
y
roslaunch um_acc setup_localize.launch