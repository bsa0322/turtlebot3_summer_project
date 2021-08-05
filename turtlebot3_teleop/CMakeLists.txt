################################################################################
# Set minimum required version of cmake, project name and compile options
################################################################################
cmake_minimum_required(VERSION 2.8.3)
project(turtlebot3_teleop)

################################################################################
# Find catkin packages and libraries for catkin and system dependencies
################################################################################
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  geometry_msgs
)

################################################################################
# Setup for python modules and scripts
################################################################################
catkin_python_setup()

################################################################################
# Declare ROS messages, services and actions
################################################################################

################################################################################
# Declare ROS dynamic reconfigure parameters
################################################################################

################################################################################
# Declare catkin specific configuration to be passed to dependent projects
################################################################################
catkin_package(
  INCLUDE_DIRS src
  CATKIN_DEPENDS roscpp geometry_msgs
  CATKIN_DEPENDS rospy geometry_msgs
)

################################################################################
# Build
################################################################################
include_directories(${catkin_INCLUDE_DIRS})

add_executable(turtlebot3_teleop_example nodes/turtlebot3_teleop_example.cpp)
target_link_libraries(turtlebot3_teleop_example ${catkin_LIBRARIES})

add_executable(turtlebot3_to_goal nodes/turtlebot3_to_goal.cpp)
target_link_libraries(turtlebot3_to_goal ${catkin_LIBRARIES})

add_executable(turtlebot3_xy_goal nodes/turtlebot3_xy_goal.cpp)
target_link_libraries(turtlebot3_xy_goal ${catkin_LIBRARIES})

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")


################################################################################
# Install
################################################################################
catkin_install_python(PROGRAMS
  nodes/turtlebot3_teleop_key
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

################################################################################
# Test
################################################################################
