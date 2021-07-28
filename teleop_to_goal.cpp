#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/String.h>

// Reminder message
const char* msg = R"(

This code move turtlebot straight.

)";

// Init variables
float target_linear_vel   = 0.1;
double current_x = 0; 
double goal_x = 0.5;

void odomCallback(const nav_msgs::Odometry::ConstPtr& odom)
{
  ROS_INFO("I received odom: [%f, %f, %f]", odom->pose.pose.position.x, odom->pose.pose.position.y, odom->pose.pose.position.z); //받았다는 걸 명시...!
  current_x = odom->pose.pose.position.x;
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

  while(current_x < goal_x){

    // Update the Twist message
    twist.linear.x = target_linear_vel;
    twist.linear.y = 0.0;
    twist.linear.z = 0.0;

    twist.angular.x = 0.0;
    twist.angular.y = 0.0;
    twist.angular.z = 0.0;

    // Publish it and resolve any remaining callbacks
    pub.publish(twist);
    ros::spinOnce();
    
  }

  twist.linear.x = 0;
  pub.publish(twist);

  return 0;
}


