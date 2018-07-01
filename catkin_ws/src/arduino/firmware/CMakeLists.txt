cmake_minimum_required(VERSION 2.8.3)

include_directories(${ROS_LIB_DIR})

#add_definitions(-DUSB_CON)

generate_arduino_firmware(line
  SRCS arduino_mecanum_node_pid.cpp ${ROS_LIB_DIR}/time.cpp
  BOARD mega2560
  PORT /dev/ttyACM0
)
