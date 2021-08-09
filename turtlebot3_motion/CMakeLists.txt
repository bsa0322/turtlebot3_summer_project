cmake_minimum_required(VERSION 3.0.2)
project(turtlebot3_motion)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  geometry_msgs
  nav_msgs
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES turtlebot3_motion
  CATKIN_DEPENDS roscpp std_msgs
  CATKIN_DEPENDS roscpp geometry_msgs
  CATKIN_DEPENDS roscpp nav_msgs
)

include_directories(${catkin_INCLUDE_DIRS})

add_executable(turtlebot3_xy_goal_amcl src/turtlebot3_xy_goal_amcl.cpp)
target_link_libraries(turtlebot3_xy_goal_amcl ${catkin_LIBRARIES})

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

