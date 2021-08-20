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
  ros::Rate loop_rate(10);

  ros::spinOnce();

  double goal_ang = std::atan2(goal_y - current_y , goal_x - current_x); //골의 세타, 단위는 라디안
  double x_direc = goal_x - current_x;
  double y_direc = goal_y - current_y;
  if(x_direc > 0) x_direc_orig = 1;
  else x_direc_orig = 0;
  if(y_direc > 0) y_direc_orig = 1;
  else y_direc_orig = 0;

  printf("골의 세타: %f\n",goal_ang);

  while(std::abs(goal_x-current_x) > 0.01 || std::abs(goal_y-current_y) > 0.01)
  {
    x_direc = goal_x - current_x;
    y_direc = goal_y - current_y;
    if(x_direc > 0) x_direc = 1;
    else x_direc = 0;
    if(y_direc > 0) y_direc = 1;
    else y_direc = 0;

    if(std::abs(goal_ang - current_angular_z) > 0.005)
    {
      printf("angular!\n");
      double ang_speed = goal_ang - current_angular_z; //+가 좌회전, -가 우회전
      if(ang_speed > 0.5) ang_speed = 0.5;
      if(ang_speed < -0.5) ang_speed = -0.5;
      twist.linear.x = 0;
      twist.linear.y = 0;
      twist.linear.z = 0;
      twist.angular.x = 0;
      twist.angular.y = 0;
      twist.angular.z = ang_speed; 
    }
    
    else
    {
      double lin_speed = 0.05; //직선
      if(x_direc != x_direc_orig && y_direc != y_direc_orig) lin_speed *= (-1); //목표지점 벗어나면 후진
      twist.linear.x = lin_speed;
      twist.linear.y = 0;
      twist.linear.z = 0;
      twist.angular.x = 0;
      twist.angular.y = 0;
      twist.angular.z = 0;
    }

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
