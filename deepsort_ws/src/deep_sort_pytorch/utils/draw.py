import rospy
from geometry_msgs.msg import Twist
from deep_sort_pytorch.msg import Bound
import numpy as np
import cv2

palette = (2 ** 11 - 1, 2 ** 15 - 1, 2 ** 20 - 1)

def compute_color_for_labels(label):
    """
    Simple function that adds fixed color depending on the class
    """
    color = [int((p * (label ** 2 - label + 1)) % 255) for p in palette]
    return tuple(color)


def draw_boxes(width, height, img, bbox, identities=None, offset=(0,0)):
    rospy.init_node('draw', anonymous=True)
    for i,box in enumerate(bbox):
        x1,y1,x2,y2 = [int(i) for i in box]
        x1 += offset[0]
        x2 += offset[0]
        y1 += offset[1]
        y2 += offset[1]

        x1_pub = x1 / width
        x2_pub = x2 / width 
        y1_pub = y1 / height
        y2_pub = y2 / height
      
        #print("카메라 크기",img.size)
        # box text and bar
        id = int(identities[i]) if identities is not None else 0    
        color = compute_color_for_labels(id)
        label = '{}{:d}'.format("", id)
        t_size = cv2.getTextSize(label, cv2.FONT_HERSHEY_PLAIN, 2 , 2)[0]
        cv2.rectangle(img,(x1, y1),(x2,y2),color,3)
        cv2.rectangle(img,(x1, y1),(x1+t_size[0]+3,y1+t_size[1]+4), color,-1)
        cv2.putText(img,label,(x1,y1+t_size[1]+4), cv2.FONT_HERSHEY_PLAIN, 2, [255,255,255], 2)

        #토픽 전달
        bound_pub = rospy.Publisher('bound_topic', Bound, queue_size=10)
        bound_message = Bound()
        bound_message.x1 = x1_pub
        bound_message.x2 = x2_pub
        bound_message.y1 = y1_pub
        bound_message.y2 = y2_pub
        bound_pub.publish(bound_message)
        #rospy.spin()

    return img



if __name__ == '__main__':
    
    for i in range(82):
        print(compute_color_for_labels(i))
