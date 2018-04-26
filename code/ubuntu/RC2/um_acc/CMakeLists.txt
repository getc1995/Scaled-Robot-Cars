cmake_minimum_required(VERSION 2.8.3)
project(um_acc)

# add c++11
set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

# Using OpenCV
find_package( OpenCV REQUIRED )

find_package(CUDA 6.5 REQUIRED)

include_directories(${CUDA_INCLUDE_DIRS} ${CUDA_CUT_INCLUDE_DIR})

# Depenencies
find_package(catkin REQUIRED COMPONENTS roscpp rospy rosconsole std_msgs message_generation eiquadprog)

#find_package(catkin REQUIRED COMPONENTS rospy)

catkin_python_setup()

# Services and msgs
add_message_files(DIRECTORY msg FILES CarState.msg CarState_L.msg IMU.msg Motor_data.msg Lidar_data.msg CentralSignal.msg)
add_service_files(DIRECTORY srv FILES ComputeControl.srv ComputeControl_L.srv MPC_CC.srv MPC_ACC.srv MPC_LK.srv)
generate_messages(DEPENDENCIES std_msgs um_acc)

# Declare catking package
catkin_package(CATKIN_DEPENDS roscpp std_msgs message_runtime)

# Declare and build targets
add_library(kalman STATIC src/kalman.cpp)
add_library(rs232 STATIC src/rs232.cpp)
add_library(ccsolver STATIC src/ccsolver.cpp)
add_library(accsolver STATIC src/accsolver.cpp)
add_library(lksolver STATIC src/lksolver.cpp)
add_library(Car_parameters STATIC src/Car_parameters.cpp)

include_directories(cudd-2.5.0/include ${catkin_INCLUDE_DIRS} ${OpenCV_DIR})

add_definitions(-lstdc++ -lm)

#add_executable(compute_control_um src/compute_control_um.cpp)
#add_dependencies(compute_control_um um_acc_generate_messages_cpp)
#target_link_libraries(compute_control_um ${catkin_LIBRARIES} qphelper)
######

set(cudd_LIBRARIES_REL cudd dddmp epd util mtr st)
set(cudd_LIBRARIES)
foreach(lib ${cudd_LIBRARIES_REL})
  list(APPEND cudd_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/cudd-2.5.0/${lib}/lib${lib}.a)
endforeach()
#message("libs: ${cudd_LIBRARIES}")

#target_link_libraries(
 #compute_control_ucla
 #  ${catkin_LIBRARIES} 
 #  ${cudd_LIBRARIES}
 #)


#####

add_executable(remote_car src/remote_car.cpp)
add_dependencies(remote_car um_acc_generate_messages_cpp)
target_link_libraries(remote_car ${catkin_LIBRARIES})

add_executable(experiment src/experiment.cpp)
add_dependencies(experiment um_acc_generate_messages_cpp)
target_link_libraries(experiment ${catkin_LIBRARIES})

add_executable(CC_MPC src/CC_MPC.cpp)
add_dependencies(CC_MPC um_acc_generate_messages_cpp)
target_link_libraries(CC_MPC ${catkin_LIBRARIES} ccsolver)

add_executable(ACC_MPC src/ACC_MPC.cpp)
add_dependencies(ACC_MPC um_acc_generate_messages_cpp)
target_link_libraries(ACC_MPC ${catkin_LIBRARIES} accsolver)

add_executable(LK_MPC src/LK_MPC.cpp)
add_dependencies(LK_MPC um_acc_generate_messages_cpp)
target_link_libraries(LK_MPC ${catkin_LIBRARIES} lksolver)

add_executable(control_interface src/control_interface.cpp)
add_dependencies(control_interface um_acc_generate_messages_cpp)
target_link_libraries(control_interface ${catkin_LIBRARIES})

add_executable(CC_simple src/CC_simple.cpp)
add_dependencies(CC_simple um_acc_generate_messages_cpp)
target_link_libraries(Car_parameters ${catkin_LIBRARIES})
target_link_libraries(CC_simple ${catkin_LIBRARIES} Car_parameters)

add_executable(Lidar_filter src/Lidar_filter.cpp)
add_dependencies(Lidar_filter um_acc_generate_messages_cpp)
target_link_libraries(Lidar_filter ${catkin_LIBRARIES})

add_executable(Motor src/Motor.cpp)
add_dependencies(Motor um_acc_generate_messages_cpp)
target_link_libraries(rs232 ${catkin_LIBRARIES})
target_link_libraries(Motor ${catkin_LIBRARIES} rs232)

add_executable(IMU_talker src/IMU_talker.cpp)
add_dependencies(IMU_talker um_acc_generate_messages_cpp)
target_link_libraries(IMU_talker ${catkin_LIBRARIES})

add_executable(Record src/Record.cpp)
add_dependencies(Record um_acc_generate_messages_cpp)
target_link_libraries(Record ${catkin_LIBRARIES})

add_executable(manual_gui src/manual_gui.cpp)
add_dependencies(manual_gui um_acc_generate_messages_cpp)
target_link_libraries(manual_gui ${catkin_LIBRARIES} ${OpenCV_LIBS} Car_parameters)

#add_executable(lane_publisher src/lane_publisher.cpp)
#add_dependencies(lane_publisher um_acc_generate_messages_cpp)
#target_link_libraries(lane_publisher ${catkin_LIBRARIES} ${OpenCV_LIBS})

#add_executable(lane_publisher2 src/lane_publisher2.cpp)
#add_dependencies(lane_publisher2 um_acc_generate_messages_cpp)
#target_link_libraries(lane_publisher2 ${catkin_LIBRARIES} ${OpenCV_LIBS})

#add_executable(camera src/camera.cpp)
#add_dependencies(camera um_acc_generate_messages_cpp)
#target_link_libraries(camera ${catkin_LIBRARIES} ${OpenCV_LIBS})

#add_executable(camera_corridor src/camera_corridor.cpp)
#add_dependencies(camera_corridor um_acc_generate_messages_cpp)
#target_link_libraries(camera_corridor ${catkin_LIBRARIES} ${OpenCV_LIBS})

#add_executable(camcali src/camcali.cpp)
#add_dependencies(camcali um_acc_generate_messages_cpp)
#target_link_libraries(camcali ${catkin_LIBRARIES} ${OpenCV_LIBS})

#add_executable(compute_control_um_lanekeeping src/compute_control_um_lanekeeping.cpp)
#add_dependencies(compute_control_um_lanekeeping src/compute_control_um_lanekeeping.cpp)
#target_link_libraries(compute_control_um_lanekeeping ${catkin_LIBRARIES} qp_vars)

add_executable(rc_interface src/rc_interface.cpp)
add_dependencies(rc_interface um_acc_generate_messages_cpp)
target_link_libraries(rc_interface ${cudd_LIBRARIES} ${catkin_LIBRARIES} kalman)

#add_executable(controller src/controller.cpp)
#add_dependencies(controller um_acc_generate_messages_cpp)
#target_link_libraries(controller ${catkin_LIBRARIES})

#add_executable(controller_lanekeeping src/controller_lanekeeping.cpp)
#add_dependencies(controller_lanekeeping um_acc_generate_messages_cpp)
#target_link_libraries(controller_lanekeeping ${catkin_LIBRARIES})

catkin_install_python(PROGRAMS src/slam.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

catkin_install_python(PROGRAMS src/central_controller.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

catkin_install_python(PROGRAMS src/getCarStateSingle.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

catkin_install_python(PROGRAMS src/getCarStateDouble.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

## Specify libraries to link a library or executable target against