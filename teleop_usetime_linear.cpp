#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

#include <ros/time.h>
#include <stdio.h>
#include <cmath>

ros::Publisher pub;

void move(double speed, double distance, int isForward)
{
  geometry_msgs::Twist twist;

  if(isForward)
  {
    twist.linear.x = speed;
  }
  else
  {
    twist.linear.x = -speed;
  }
  twist.linear.y = 0;
  twist.linear.z = 0;
  twist.angular.x = 0;
  twist.angular.y = 0;
  twist.angular.z = 0;

  //ros::Time begin = ros::Time::now();
  double time = 0;
  while(time == 0) time = ros::Time::now().toSec();
  printf("\n%f\n", time);
  double current_distance = 0;
  //ros::Rate loop_rate(10);

  while(current_distance <= distance)
  {
    pub.publish(twist);
    double time2 = ros::Time::now().toSec();
    printf("%f\n", time2);
    current_distance = (time2-time) * speed;
    ros::spinOnce();
    //loop_rate.sleep();
  }

  twist.linear.x = 0;
  pub.publish(twist);
  
}

int main(int argc, char** argv)
{
  // Init ROS node
  ros::init(argc, argv, "teleop_twist_keyboard");
  ros::NodeHandle nh;

  // Init cmd_vel publisher
  pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1);

  double speed, distance;
  int isForward;

  std::cout<<"속도: ";
  std::cin>>speed;
  std::cout<<"\n거리: ";
  std::cin>>distance;
  std::cout<<"\n앞 뒤: ";
  std::cin>>isForward;
  
  move(speed, distance, isForward);
  
  return 0;
}

