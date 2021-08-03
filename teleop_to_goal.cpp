#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/String.h>

#include <stdio.h>
#include <cmath>

// Reminder message
const char* msg = R"(

This code move turtlebot straight.

)";

//amcl
// Init variables
double current_x; 

ros::Publisher pub;
geometry_msgs::Twist twist;

void odomCallback(const nav_msgs::Odometry::ConstPtr& odom)
{
  current_x = odom->pose.pose.position.x;
  ROS_INFO("I received odom: [%f, %f, %f]", current_x, odom->pose.pose.position.y, odom->pose.pose.position.z); //받았다는 걸 명시...!
}

void to_goal(double goal_x)
{
  float cur_speed = 0;
  ros::Rate loop_rate(10);
  while(std::abs(goal_x - current_x) > 0.001)
  {
    cur_speed = goal_x - current_x;
    if(cur_speed > 0.2) cur_speed = 0.2; //maximum speed
    if(cur_speed < -0.2) cur_speed = -0.2; //maximum -speed

    // Update the Twist message
    twist.linear.x = cur_speed;
    twist.linear.y = 0.0;
    twist.linear.z = 0.0;

    twist.angular.x = 0.0;
    twist.angular.y = 0.0;
    twist.angular.z = 0.0;

    // Publish it and resolve any remaining callbacks
    pub.publish(twist);
    ros::spinOnce();
    loop_rate.sleep();
  }
  
  printf("here\n");
  twist.linear.x = 0;
  twist.linear.y = 0;
  twist.linear.z = 0;
  twist.angular.x = 0;
  twist.angular.y = 0;
  twist.angular.z = 0;

  pub.publish(twist);
}

int main(int argc, char** argv)
{
  // Init ROS node
  ros::init(argc, argv, "turtlebot3_to_goal");
  ros::NodeHandle nh;

  // Init cmd_vel publisher
  pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

  // Init odom subsciber
  ros::Subscriber sub = nh.subscribe("/odom", 1000, odomCallback);  

  printf("%s", msg);

  double goal_x;
  
  std::cout<<"목표지점 x좌표: ";
  std::cin>>goal_x;

  to_goal(goal_x);

  return 0;
}


