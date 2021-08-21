#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/String.h>
#include <tf/transform_listener.h>

#include <stdio.h>
#include <cmath>

// Reminder message
const char* msg = R"(

This code move turtlebot to track people.

)";

ros::Publisher pub;

// Create Twist message
geometry_msgs::Twist twist;

double x1,x2,y1,y2;
double size = 0.5;



void found_people()
{
  ros::Rate loop_rate(10);
  ros::spinOnce();

  while(true)
  {
    twist.angular.z = 0.1;

    if(x1 != 0 || x2 != 0 || y1 != 0 || y2 != 0) break;

    // Publish it and resolve any remaining callbacks
    pub.publish(twist);
    ros::spinOnce();
    loop_rate.sleep();

  }

  twist.linear.x = 0;
  twist.angular.z = 0;
  pub.publish(twist);
}

void track_people()
{
  ros::Rate loop_rate(10);
  ros::spinOnce();

  twist.linear.y = twist.linear.z = twist.angular.x = twist.angular.y = 0;

  while(true)
  {
    double x = (x1+x2)/2;
    double y = (y1+y2)/2;
    double size2 = abs(x2-x1)*abs(y2-y1);

    double linear_speed = size - size2;
    if(linear_speed > 0.05) linear_speed = 0.05;
    if(linear_speed < -0.05) linear_speed = -0.05;

    double angular_speed = 0.5 - x;
    if(angular_speed > 0.2) angular_speed = 0.2;
    if(angular_speed < -0.2) angular_speed = -0.2;

    //장애물 처리

    twist.linear.x = linear_speed;
    twist.angular.z = angular_speed;
   
    // Publish it and resolve any remaining callbacks
    pub.publish(twist);
    ros::spinOnce();
    loop_rate.sleep();
  }

  twist.linear.x = 0;
  twist.angular.z = 0;
  pub.publish(twist);
}

void boundCallback(const nav_msgs::Odometry::ConstPtr& bound)
{
  //x1, x2, y1, y2 받아오기
  ROS_INFO("I received bound: [%f, %f, %f, %f]", x1, x2, y1, y2); //받았다는 걸 명시...!
}

void rangesCallback()
{
  //받아오기
}

int main(int argc, char** argv)
{
  // Init ROS node
  ros::init(argc, argv, "turtlebot_track_people");
  ros::NodeHandle nh;

  // Init cmd_vel publisher
  pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 10);

  // Init odom subsciber
  ros::Subscriber sub = nh.subscribe("bound", 1000, boundCallback);  //카메라 바운드 좌표 받아오기
  ros::Subscriber sub2 = nh.subscribe("ranges", 1000, rangesCallback); //장애물 센서 거리 받아오기

  printf("%s", msg);

  found_people();

  track_people();

  return 0;
}
