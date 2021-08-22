#include <ros/ros.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

void imageCallback(const sensor_msgs::CompressedImageConstPtr& msg)
{
  try
  {
    cv::Mat image = cv::imdecode(cv::Mat(msg->data),1);//convert compressed image data to cv::Mat
    cv::imshow("view", image);

    cv::waitKey(10);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert to image!");
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "image_listener");
  ros::NodeHandle nh;
  cv::namedWindow("view");
  //cv::waitKey();
  ros::Subscriber sub = nh.subscribe("/raspicam_node/image/compressed", 1, imageCallback);
  ros::spin();
  cv::destroyWindow("view");
}
