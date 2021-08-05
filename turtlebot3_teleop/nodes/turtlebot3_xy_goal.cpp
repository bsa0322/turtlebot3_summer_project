#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/String.h>
#include <tf/transform_listener.h>

#include <stdio.h>
#include <cmath>

// Reminder message
const char* msg = R"(

This code move turtlebot to goal(x,y).

)";

ros::Publisher pub;

// Create Twist message
geometry_msgs::Twist twist;

double current_x = 0; 
double current_y = 0;
double current_angular_z = 0;
int x_direc_orig = 0; //앞 뒤 정하기 위한 방향
int y_direc_orig = 0;

void to_goal(double goal_x, double goal_y)
{
  ros::Rate loop_rate(100);

  ros::spinOnce();

  while(true)
  {
    double goal_ang = std::atan2(goal_y - current_y , goal_x - current_x); //골의 세타, 단위는 라디안
    double distance = sqrt(pow(goal_x - current_x, 2) + pow(goal_y - current_y, 2)); //현재 위치와 목표 위치 사이의 거리
    printf("골의 세타: %f, 남은 거리: %f\n",goal_ang,distance);

    if(distance < 0.01) break;

    double ang_speed = goal_ang - current_angular_z;
    double lin_speed = distance;
    if(ang_speed > 0.5) ang_speed = 0.5;
    if(ang_speed < -0.5) ang_speed = -0.5;
    if(lin_speed > 0.2) lin_speed = 0.2;
    if(lin_speed < -0.2) lin_speed = -0.2;

    twist.linear.x = lin_speed;
    twist.linear.y = 0;
    twist.linear.z = 0;
    twist.angular.x = 0;
    twist.angular.y = 0;
    twist.angular.z = ang_speed;

    // Publish it and resolve any remaining callbacks
    pub.publish(twist);
    ros::spinOnce();
    loop_rate.sleep();
  }

  twist.linear.x = 0;
  twist.angular.z = 0;
  pub.publish(twist);
}

void odomCallback(const nav_msgs::Odometry::ConstPtr& odom)
{
  current_x = odom->pose.pose.position.x;
  current_y = odom->pose.pose.position.y;
  tf::Quaternion q(odom->pose.pose.orientation.x, odom->pose.pose.orientation.y, odom->pose.pose.orientation.z, odom->pose.pose.orientation.w);
  tf::Matrix3x3 m(q);
  double roll, pitch, yaw;
  m.getRPY(roll, pitch, yaw);
  current_angular_z = yaw; //단위는 라디안
  ROS_INFO("I received odom: [%f, %f, %f]", current_x, current_y, current_angular_z); //받았다는 걸 명시...!
}

int main(int argc, char** argv)
{
  // Init ROS node
  ros::init(argc, argv, "turtlebot3_xy_goal");
  ros::NodeHandle nh;

  // Init cmd_vel publisher
  pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 10);

  // Init odom subsciber
  ros::Subscriber sub = nh.subscribe("odom", 1000, odomCallback);  

  printf("%s", msg);

  double goal_x, goal_y;

  std::cout<<"목표 x 좌표: ";
  std::cin>>goal_x;
  std::cout<<"목표 y 좌표: ";
  std::cin>>goal_y;

  to_goal(goal_x,goal_y);

  return 0;
}

