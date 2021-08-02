#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/String.h>

#include <cmath>

// Reminder message
const char* msg = R"(

This code move turtlebot straight.

)";

// Init variables
#define LINEAR_VEL_SIZE 0.05
#define ANGULAR_VEL_SIZE 0.07

double current_x = 0; 
double current_y = 0;
double current_angular_z = 0;
double goal_x = 0.4; //골 x좌표
double goal_y = 0.3; //골 y좌표
double goal_ang; //골 angular

void odomCallback(const nav_msgs::Odometry::ConstPtr& odom)
{
  ROS_INFO("I received odom: [%f, %f, %f]", odom->pose.pose.position.x, odom->pose.pose.position.y, odom->pose.pose.orientation.z); //받았다는 걸 명시...!
  current_x = odom->pose.pose.position.x;
  current_y = odom->pose.pose.position.y;
  current_angular_z = odom->pose.pose.orientation.z;

}

int main(int argc, char** argv)
{
  // Init ROS node
  ros::init(argc, argv, "turtlebot3_to_goal");
  ros::NodeHandle nh;

  // Init cmd_vel publisher
  ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1);
  // Create Twist message
  geometry_msgs::Twist twist;

  // Init odom subsciber
  ros::Subscriber sub = nh.subscribe("odom", 1000, odomCallback);  

  printf("%s", msg);

  goal_ang = std::atan(goal_y/goal_x) / 2;

  while(abs(goal_x-current_x) > 0.02 || abs(goal_y-current_y) > 0.02){

    if(current_angular_z <= goal_ang)
    {
      twist.linear.x = 0;
      twist.angular.z = ANGULAR_VEL_SIZE;
    }

    else //y축 - 쪽 angular
    {
      twist.linear.x = LINEAR_VEL_SIZE;
      twist.angular.z = 0;
    }

    // Publish it and resolve any remaining callbacks
    pub.publish(twist);
    ros::spinOnce();
    
  }

  twist.linear.x = 0;
  twist.angular.z = 0;
  pub.publish(twist);

  return 0;
}
