#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <deep_sort_pytorch/Bound.h>
#include <std_msgs/String.h>
#include <sensor_msgs/LaserScan.h>
#include <tf/transform_listener.h>

#include <stdio.h>
#include <vector>
#include <cmath>

// Reminder message
const char* msg = R"(

This code move turtlebot to track people.

)";

ros::Publisher pub;

// Create Twist message
geometry_msgs::Twist twist;

double _x1, _x2, _y1, _y2;
double size = 0.5;

std::vector <float> _ranges;

void found_people()
{
  ros::Rate loop_rate(10);
  ros::spinOnce();

  while(true)
  {
    twist.angular.z = 0.1;

    if(_x1 != 0 || _x2 != 0 || _y1 != 0 || _y2 != 0) break;

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
    double x = (_x1+_x2)/2;
    double y = (_y1+_y2)/2;
    double size2 = fabs(_x2-_x1)*fabs(_y2-_y1);

    double linear_speed = size - size2;
    if(linear_speed > 0.05) linear_speed = 0.05;
    if(linear_speed < -0.05) linear_speed = -0.05;

    double angular_speed = 0.5 - x;
    if(angular_speed > 0.2) angular_speed = 0.2;
    if(angular_speed < -0.2) angular_speed = -0.2;

    //장애물 처리
    if(linear_speed > 0 && _ranges[0] < 0.5)
    {
      linear_speed *= (-1);
    }
    else if(linear_speed < 0 && _ranges[180] < 0.5)
    {
      linear_speed *= (-1);
    }

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

void boundCallback(const deep_sort_pytorch::Bound::ConstPtr& bound)
{
  _x1 = bound->x1;
  _x2 = bound->x2;
  _y1 = bound->y1;
  _y2 = bound->y2;

  ROS_INFO("I received bound: [%lf, %lf, %lf, %lf]", _x1, _x2, _y1, _y2); //받았다는 걸 명시...!
}

void rangesCallback(const sensor_msgs::LaserScan::ConstPtr& laser)
{
  //받아오기
  _ranges = laser->ranges;

  ROS_INFO("I received ranges: [%f, %f]", _ranges[0], _ranges[360]);
}

int main(int argc, char** argv)
{
  // Init ROS node
  ros::init(argc, argv, "turtlebot_track_people");
  ros::NodeHandle nh;

  // Init cmd_vel publisher
  pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 10);

  // Init odom subsciber
  ros::Subscriber sub = nh.subscribe("bound_topic", 1000, boundCallback);  //카메라 바운드 좌표 받아오기
  ros::Subscriber sub2 = nh.subscribe("/scan", 1000, rangesCallback); //장애물 센서 거리 받아오기

  printf("%s", msg);

  found_people();

  track_people();

  return 0;
}
