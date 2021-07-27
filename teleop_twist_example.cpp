#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/String.h>

#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#include <algorithm>
#include <cmath>
#include <map>

// Reminder message
const char* msg = R"(

This code move turtlebot straight.

)";

#define LIN_VEL_STEP_SIZE 0.01
#define ANG_VEL_STEP_SIZE 0.1

float makeSimpleProfile(float output, float input, float slop)
{
  if (input > output)
    output = std::min( input, output + slop );
  else if (input < output)
    output = std::max( input, output - slop );
  else
    output = input;

  return output;
}

void odomCallback(const nav_msgs::Odometry::ConstPtr& odom)
{
  ROS_INFO("I received odom: [%f, %f, %f]", odom->twist.twist.linear.x, odom->pose.pose.position.y, odom->pose.pose.position.z); //받았다는 걸 명시...!
}

int main(int argc, char** argv)
{
  // Init ROS node
  ros::init(argc, argv, "turtlebot3_to_goal");
  ros::NodeHandle nh;

  // Init cmd_vel publisher
  ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1);

  // Init odom subsciber
  ros::Subscriber sub = nh.subscribe("odom", 1000, odomCallback);

  // Create Twist message
  geometry_msgs::Twist twist;

  // Init variables
  float target_linear_vel   = 0.0;
  float target_angular_vel  = 0.0;
  float control_linear_vel  = 0.0;
  float control_angular_vel = 0.0;

  double current_x = odom.pose.pose.position.x;
  double current_y = odom.pose.pose.position.y;
  double current_z = odom.pose.pose.position.z;

  double goal_x = 0.1;

  printf("%s", msg);

  while(current_x < goal_x){

    target_linear_vel = LIN_VEL_STEP_SIZE;

    control_linear_vel = makeSimpleProfile(control_linear_vel, target_linear_vel, (LIN_VEL_STEP_SIZE/2.0));
    control_angular_vel = makeSimpleProfile(control_angular_vel, target_angular_vel, (ANG_VEL_STEP_SIZE/2.0));

    // Update the Twist message
    twist.linear.x = control_linear_vel;
    twist.linear.y = 0.0;
    twist.linear.z = 0.0;

    twist.angular.x = 0.0;
    twist.angular.y = 0.0;
    twist.angular.z = control_angular_vel;

    // Publish it and resolve any remaining callbacks
    pub.publish(twist);
    ros::spinOnce();
  }

  return 0;
}


