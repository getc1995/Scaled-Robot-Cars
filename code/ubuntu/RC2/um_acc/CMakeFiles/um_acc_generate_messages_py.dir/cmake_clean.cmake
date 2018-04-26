FILE(REMOVE_RECURSE
  "CMakeFiles/um_acc_generate_messages_py"
  "devel/lib/python2.7/dist-packages/um_acc/msg/_CarState_L.py"
  "devel/lib/python2.7/dist-packages/um_acc/msg/_ControlSignal.py"
  "devel/lib/python2.7/dist-packages/um_acc/msg/_CarState.py"
  "devel/lib/python2.7/dist-packages/um_acc/msg/_CarStateExtended.py"
  "devel/lib/python2.7/dist-packages/um_acc/msg/_Lane.py"
  "devel/lib/python2.7/dist-packages/um_acc/msg/_CarSignal.py"
  "devel/lib/python2.7/dist-packages/um_acc/msg/_Sensor.py"
  "devel/lib/python2.7/dist-packages/um_acc/msg/_Pwm.py"
  "devel/lib/python2.7/dist-packages/um_acc/srv/_ComputeControl.py"
  "devel/lib/python2.7/dist-packages/um_acc/srv/_ComputeControl_L.py"
  "devel/lib/python2.7/dist-packages/um_acc/msg/__init__.py"
  "devel/lib/python2.7/dist-packages/um_acc/srv/__init__.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/um_acc_generate_messages_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
