#!/usr/bin/env python

import rospy
from sensor_msgs.msg import Image
from std_msgs.msg import Float64
from cv_bridge import CvBridge
import cv2

from Image_process import *
from Utils import *
import time
import math

def processed_image_pub():
    pub_image = rospy.Publisher('/robocon2018/image_raw', Image, queue_size=10)
    pub_angle = rospy.Publisher('/robocon2018/angle', Float64, queue_size=10)
    pub_distance = rospy.Publisher('/robocon2018/distance', Float64, queue_size=10)

    rospy.init_node('process_image_node', anonymous=False)
    rate = rospy.Rate(15)
    
    ############################IP HERE######################################
    font = cv2.FONT_HERSHEY_SIMPLEX
    Images=[]
    N_SLICES = 4

    for q in range(N_SLICES):
        Images.append(Image_process())

    capture = cv2.VideoCapture(1)
    br = CvBridge()
    it = 1

    while not rospy.is_shutdown():
        flag, img = capture.read()
        
        direction = 0
        img = RemoveBackground(img, False)

        if img is not None:
            t1 = time.clock()
            SlicePart(img, Images, N_SLICES)
            #for i in range(N_SLICES):
            #    direction += Images[i].dir
            
            #negative error: right of middle
            #positive error: left of middle
            #error1 = Images[0].dir  #
            #error2 = Images[1].dir  
            #error3 = Images[2].dir
            #error4 = Images[3].dir  #error of furthest part of line

            if Images[3].x_coord and Images[0].x_coord:
                slope = (Images[3].x_coord - Images[0].x_coord)/180.0
                print('Slope: ', slope)
            else:
                rospy.loginfo("Could not detect contours")
            
            theta = round(math.degrees(math.atan(slope)), 2)
            delta=int(Images[1].middleX-Images[1].contourCenterX)
            fm = RepackImages(Images)
            t2 = time.clock()
            cv2.putText(fm, "Time: " + str((t2-t1)*1000) + " ms", (10, 470), font, 0.5, (0,0,255), 1, cv2.LINE_AA)
            
            # for i in range(N_SLICES):
            #     cv2.imshow("part %d" % i, Images[i].image)
            #     print('error1: ', error1)
            #     print('error2: ', error2)
            #     print('error3: ', error3)
            #     print('error4: ', error4, end="\n\n")

            # cv2.imshow("frame", fm)

            print('Angle: ', theta)
            print('Distance: ', delta)
            print('Iteration: ', it)

            it = it + 1

        rospy.loginfo('publishing video stream')
        pub_image.publish(br.cv2_to_imgmsg(fm, "bgr8"))

        rospy.loginfo('publishing angle')
        pub_angle.publish(theta)

        rospy.loginfo('publishing distance')
        pub_distance.publish(delta)

        rate.sleep()

if __name__ == '__main__':
    try:
        processed_image_pub()
    except rospy.ROSInterruptException:
        pass
