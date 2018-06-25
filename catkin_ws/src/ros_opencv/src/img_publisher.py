#!/usr/bin/env python

import rospy
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2

def img_talker():
    pub = rospy.Publisher('imager', Image, queue_size=10)
    rospy.init_node('img_talker', anonymous=True)
    rate = rospy.Rate(60)
    capture = cv2.VideoCapture(0)
    br = CvBridge()
    while not rospy.is_shutdown():
        ret, img = capture.read()
        rospy.loginfo('publishing image')
        pub.publish(br.cv2_to_imgmsg(img, "bgr8"))
        rate.sleep()

if __name__ == '__main__':
    try:
        img_talker()
    except rospy.ROSInterruptException:
        pass