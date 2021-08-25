import os
import cv2
import time
import argparse
from cv2 import data
import torch
import warnings
import numpy as np

import rospy
import sys

from std_msgs.msg import String
from sensor_msgs.msg import CompressedImage
from cv_bridge import CvBridge, CvBridgeError

from detector import build_detector
from deep_sort import build_tracker
from utils.draw import draw_boxes
from utils.parser import get_config
from utils.log import get_logger
from utils.io import write_results

_video_path=None
chk=0

class VideoTracker(object):
    def __init__(self, cfg, args):
        self.cfg = cfg
        self.args = args
        
        self.logger = get_logger("root")

        use_cuda = args.use_cuda and torch.cuda.is_available()
        if not use_cuda:
            warnings.warn("Running in cpu mode which maybe very slow!", UserWarning)

        if args.display:
            cv2.namedWindow("test", cv2.WINDOW_NORMAL)
            cv2.resizeWindow("test", args.display_width, args.display_height)
            
        if args.cam != -1:
            print("Using webcam " + str(args.cam))
        
        self.detector = build_detector(cfg, use_cuda=use_cuda)
        self.deepsort = build_tracker(cfg, use_cuda=use_cuda)
        self.class_names = self.detector.class_names

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, exc_traceback):
        if exc_type:
            print(exc_type, exc_value, exc_traceback)

    def run(self):
        while True:
            results = []
            idx_frame = 0
            
            #while True:
            #idx_frame += 1
            #if idx_frame % self.args.frame_interval:
            #    continue

            start = time.time()

            #_, ori_im = self.vdo.retrieve()

            #ros->opencv
                
            ori_im = None;
            if chk == 0:
                continue
            ori_im = cv2.imdecode(_video_path, cv2.IMREAD_COLOR) 
            print("video path: ",_video_path)
            if not ori_im.data == False:
                
                print("Image found")
                #break
                
                #cv2.imshow('cv_img',ori_im)
                #cv2.waitKey(2) 
                im = cv2.cvtColor(ori_im, cv2.COLOR_BGR2RGB)
                    

                #카메라 크기 얻어옴
                width = np.size(ori_im, 1)
                height = np.size(ori_im, 0)
                print("카메라 크기",width,height)
                        
                # do detection
                bbox_xywh, cls_conf, cls_ids = self.detector(im)

                # select person class
                mask = cls_ids == 0

                bbox_xywh = bbox_xywh[mask]
                # bbox dilation just in case bbox too small, delete this line if using a better pedestrian detector
                bbox_xywh[:, 3:] *= 1.2                   
                cls_conf = cls_conf[mask]

                # do tracking
                outputs = self.deepsort.update(bbox_xywh, cls_conf, im)

                print("outputs: ", outputs)
                # draw boxes for visualization
                if len(outputs) > 0:
                    bbox_tlwh = []
                    bbox_xyxy = outputs[:, :4]
                    identities = outputs[:, -1]
                    ori_im = draw_boxes(width, height, ori_im, bbox_xyxy, identities)

                    for bb_xyxy in bbox_xyxy:
                        bbox_tlwh.append(self.deepsort._xyxy_to_tlwh(bb_xyxy))

                    results.append((idx_frame - 1, bbox_tlwh, identities))

                end = time.time()

                cv2.imshow("test", ori_im)
                cv2.waitKey(2)

                # logging
                self.logger.info("time: {:.03f}s, fps: {:.03f}, detection numbers: {}, tracking numbers: {}" \
                                    .format(end - start, 1 / (end - start), bbox_xywh.shape[0], len(outputs)))


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("VIDEO_PATH", type=str)
    parser.add_argument("--config_detection", type=str, default="./configs/yolov3.yaml")
    parser.add_argument("--config_deepsort", type=str, default="./configs/deep_sort.yaml")
    # parser.add_argument("--ignore_display", dest="display", action="store_false", default=True)
    parser.add_argument("--display", action="store_true")
    parser.add_argument("--frame_interval", type=int, default=1)
    parser.add_argument("--display_width", type=int, default=800)
    parser.add_argument("--display_height", type=int, default=600)
    parser.add_argument("--save_path", type=str, default="./output/")
    parser.add_argument("--cpu", dest="use_cuda", action="store_false", default=True)
    parser.add_argument("--camera", action="store", dest="cam", type=int, default="-1")
    return parser.parse_args()


#ros -> opencv
def callback(ros_data):
    global chk
    chk=1
    #rate = rospy.Rate(10)
    global _video_path
    _video_path = np.fromstring(ros_data.data, np.uint8)
    print(_video_path)
    #rate.sleep()

if __name__ == "__main__":
    #ros->opencv
    args = parse_args()
    cfg = get_config()
    cfg.merge_from_file(args.config_detection)
    cfg.merge_from_file(args.config_deepsort)
    vt = VideoTracker(cfg, args)

    rospy.init_node('image_feature', anonymous=True)
    subscriber = rospy.Subscriber("/raspicam_node/image/compressed", CompressedImage, callback, queue_size = 1)
    
    vt.run()
    
    rate = rospy.Rate(1)
    rospy.spin()
    rate.sleep()

    

    


