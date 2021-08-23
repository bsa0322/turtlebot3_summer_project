cmake_minimum_required(VERSION 2.8.3)
project(deep_sort_pytorch)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  rospy
  roscpp
  std_msgs
  nav_msgs
  sensor_msgs
  actionlib_msgs
  cv_bridge
  image_transport
  message_generation
)


## Generate messages in the 'msg' folder
add_message_files(
  FILES 
  Bound.msg
)

## Generate added messages and services with any dependencies listed here
 generate_messages(
   DEPENDENCIES
   std_msgs
   actionlib_msgs
   deep_sort_pytorch
 )


###################################
## catkin specific configuration ##
###################################
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES deep_sort_pytorch
  CATKIN_DEPENDS rospy roscpp std_msgs message_runtime
  EXPORTED_TARGETS deep_sort_pytorch_generate_messages_cpp
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(turtlebot_track_people turtlebot_track_people.cpp)
target_link_libraries(turtlebot_track_people ${catkin_LIBRARIES})
add_dependencies(turtlebot_track_people ${catkin_EXPORTED_TARGETS})

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

catkin_install_python(PROGRAMS
  utils/draw.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
