#!/usr/bin/env python

import rospy
import cv2
import numpy as np
from scipy.ndimage import filters
from std_msgs.msg import String
#from scipy.ndimage import filters
from sensor_msgs.msg import CompressedImage
from cv_bridge import CvBridge, CvBridgeError
import sys, time

VERBOSE=False

class image_feature:
      def __init__(self):
            self.subscriber = rospy.Subscriber("/raspicam_node/image/compressed", CompressedImage, self.callback, queue_size = 1)
            if VERBOSE:
                  print("subscribed to /raspicam_node/image/compressed")
      def callback(self, ros_data):
        if VERBOSE:
          print('received image of type: "%s"' % ros_data.format)

        np_arr = np.fromstring(ros_data.data, np.uint8)
        image_np = cv2.imdecode(np_arr, cv2.IMREAD_COLOR)

        
        cv2.imshow('cv_img',image_np)
        cv2.waitKey(2)

def main(args):
    '''Initializes and cleanup ros node'''
    ic = image_feature()
    rospy.init_node('image_feature', anonymous=True)
    try:
        rospy.spin()
    except KeyboardInterrupt:
        print ("Shutting down ROS Image feature detector module")
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main(sys.argv)


