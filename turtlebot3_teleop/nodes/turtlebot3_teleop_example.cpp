#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

#include <ros/time.h>
#include <stdio.h>
#include <cmath>

ros::Publisher pub; 
geometry_msgs::Twist twist;

void move(double speed, double distance, int isForward)
{
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
  ros::Rate loop_rate(10); //이거 없으면 실제 로봇에서 적용 안됨, 안 멈춤.

  while(current_distance <= distance)
  {
    double time2 = ros::Time::now().toSec();
    printf("%f\n", time2);
    current_distance = (time2-time) * speed;
    pub.publish(twist);
    ros::spinOnce(); //없어도 잘 실행되긴 함.
    loop_rate.sleep(); //이거 없으면 실제 로봇에서 적용 안됨, 안 멈춤.
  }

  //twist = geometry_msgs::Twist(); //twist 초기화,,?!
  twist.linear.x = 0;
  twist.linear.y = 0;
  twist.linear.z = 0;
  twist.angular.x = 0;
  twist.angular.y = 0;
  twist.angular.z = 0;

  pub.publish(twist);
  //ros::spin();

  printf("x선 속도: %f\n",twist.linear.x);
  
}

int main(int argc, char** argv)
{
  // Init ROS node
  ros::init(argc, argv, "teleop_twist_keyboard");
  ros::NodeHandle nh;

  // Init cmd_vel publisher
  pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 10); //topic에 /있으나 없으나 상관 없다.

  double speed, distance;
  int isForward;

  std::cout<<"속도: ";
  std::cin>>speed;
  std::cout<<"\n거리: ";
  std::cin>>distance;
  std::cout<<"\n앞 뒤: ";
  std::cin>>isForward;
  
  move(speed, distance, isForward);

  //ros::spin();
  
  return 0;
}
