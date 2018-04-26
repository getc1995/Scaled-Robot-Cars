FILE(REMOVE_RECURSE
  "CMakeFiles/um_acc_generate_messages_cpp"
  "devel/include/um_acc/CarState_L.h"
  "devel/include/um_acc/ControlSignal.h"
  "devel/include/um_acc/CarState.h"
  "devel/include/um_acc/CarStateExtended.h"
  "devel/include/um_acc/Lane.h"
  "devel/include/um_acc/CarSignal.h"
  "devel/include/um_acc/Sensor.h"
  "devel/include/um_acc/Pwm.h"
  "devel/include/um_acc/ComputeControl.h"
  "devel/include/um_acc/ComputeControl_L.h"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/um_acc_generate_messages_cpp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
