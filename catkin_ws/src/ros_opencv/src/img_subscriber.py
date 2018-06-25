#!/usr/bin/env python
import rospy
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2

def callback(data):
    br = CvBridge()
    rospy.loginfo('receiving Image')
    cv2.imshow('Image', br.imgmsg_to_cv2(data, 'passthrough'))
    cv2.waitKey(1)

def img_listener():
    rospy.init_node('img_listener', anonymous=True)
    rospy.Subscriber('/usb_cam/image_raw', Image, callback)
    rospy.spin()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    img_listener()