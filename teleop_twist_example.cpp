#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#include <algorithm>
#include <cmath>
#include <map>

// Reminder message
const char* msg = R"(

Control Your TurtleBot3!
---------------------------
Moving around:
        w
   a    s    d
        x

w/x : increase/decrease linear velocity (Burger : ~ 0.22, Waffle and Waffle Pi : ~ 0.26)
a/d : increase/decrease angular velocity (Burger : ~ 2.84, Waffle and Waffle Pi : ~ 1.82)

space key, s : force stop

CTRL-C to quit

)";

#define LIN_VEL_STEP_SIZE 0.01
#define ANG_VEL_STEP_SIZE 0.1

// For non-blocking keyboard inputs
int getch(void)
{
  int ch;
  struct termios oldt;
  struct termios newt;

  // Store old settings, and copy to new settings
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;

  // Make required changes and apply the settings
  newt.c_lflag &= ~(ICANON | ECHO);
  newt.c_iflag |= IGNBRK;
  newt.c_iflag &= ~(INLCR | ICRNL | IXON | IXOFF);
  newt.c_lflag &= ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);
  newt.c_cc[VMIN] = 1;
  newt.c_cc[VTIME] = 0;
  tcsetattr(fileno(stdin), TCSANOW, &newt);

  // Get the current character
  ch = getchar();

  // Reapply old settings
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  return ch;
}

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

int main(int argc, char** argv)
{
  // Init ROS node
  ros::init(argc, argv, "teleop_twist_keyboard");
  ros::NodeHandle nh;

  // Init cmd_vel publisher
  ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1);

  // Create Twist message
  geometry_msgs::Twist twist;

  // Init variables
  float target_linear_vel   = 0.0;
  float target_angular_vel  = 0.0;
  float control_linear_vel  = 0.0;
  float control_angular_vel = 0.0;
  char key = ' ';

  printf("%s", msg);

  while(true){

    // Get the pressed key
    key = getch();

    if(key == 'w')
    {
      target_linear_vel+=LIN_VEL_STEP_SIZE;
    }
    else if(key == 'x')
    {
      target_linear_vel-=LIN_VEL_STEP_SIZE;
    }
    else if(key=='a')
    {
      target_angular_vel+=ANG_VEL_STEP_SIZE;
    }
    else if(key=='d')
    {
      target_angular_vel-=ANG_VEL_STEP_SIZE;
    }
    else if(key == ' ' || key == 's')
    {
      target_linear_vel   = 0.0;
      control_linear_vel  = 0.0;
      target_angular_vel  = 0.0;          
      control_angular_vel = 0.0;
    }
    else if(key == '\x03')
    {
      break;
    }

    control_linear_vel = makeSimpleProfile(control_linear_vel, target_linear_vel, (LIN_VEL_STEP_SIZE/2.0));
    control_angular_vel = makeSimpleProfile(control_angular_vel, target_angular_vel, (ANG_VEL_STEP_SIZE/2.0));
    
    printf("\rCurrent: speed %f\tturn %f | Invalid command! %c", control_linear_vel, control_angular_vel, key);

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

