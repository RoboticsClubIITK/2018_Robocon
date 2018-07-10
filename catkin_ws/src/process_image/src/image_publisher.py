#!/usr/bin/env python

import rospy
from sensor_msgs.msg import Image
from std_msgs.msg import Float64
from std_msgs.msg import Int32
from cv_bridge import CvBridge
import cv2
import imutils
from datetime import datetime

from Image_process import *
from Utils import *
from detect_cross import *
import time
import math

cameraNum=1

def processed_image_pub():
    rotCon=0
    sleepCon=0
    pub_image = rospy.Publisher('/robocon2018/image_raw', Image, queue_size=10)
    pub_angle = rospy.Publisher('/robocon2018/angle', Float64, queue_size=10)
    pub_distance = rospy.Publisher('/robocon2018/distance', Float64, queue_size=10)
    pub_rotation = rospy.Publisher('/robocon2018/rotation', Int32, queue_size=10)
    rospy.init_node('process_image_node', anonymous=False)
    rate = rospy.Rate(30)
    
    ############################IP HERE######################################
    font = cv2.FONT_HERSHEY_SIMPLEX
    Images=[]
    N_SLICES = 4

    for q in range(N_SLICES):
        Images.append(Image_process())

    capture = cv2.VideoCapture(cameraNum)
    br = CvBridge()
    it = 1
    start=datetime.now()

    while not rospy.is_shutdown():
        ret, img = capture.read()
        ###############################CROSS DETECTION######################
        imgfil=filter(img)
        detCon=detectIntersect(imgfil)
        if detCon==1:
            diff=datetime.now()-start
            diffms=diff.days*86400000 + diff.seconds *1000 + diff.microseconds/1000
            if (diffms)>1500:
                start=datetime.now()
                if rotCon==0:
                    rotCon=1
                elif rotCon==1:
                    rotCon=2
                elif rotCon==2:
                    rotCon=3
                    sleepCon=1
                elif rotCon==3:
                    rotCon=4
                elif rotCon==4:
                    rotCon=5
                elif rotCon==5:
                    rotCon=6
                elif rotCon==6:
                    rotCon=0
        ####################################################################
        if rotCon==1 or rotCon==2:
            img=imutils.rotate_bound(img,270)
        elif rotCon==4 or rotCon==5 or rotCon==6:
            img=imutils.rotate_bound(img,90)
        #img = RemoveBackground(img, False)

        if img is not None:
            SlicePart(img, Images, N_SLICES)
            #for i in range(N_SLICES):
            #    direction += Images[i].dir
            
            #negative error: right of middle
            #positive error: left of middle
            #error1 = Images[0].dir  #
            #error2 = Images[1].dir  
            #error3 = Images[2].dir
            #error4 = Images[3].dir  #error of furthest part of line
            slope =0
            if Images[3].x_coord and Images[0].x_coord:
                slope = (Images[3].x_coord - Images[0].x_coord)/180.0
                print('Slope: ', slope)
            else:
                rospy.loginfo("Could not detect contours")
            
            theta = round(math.atan(slope)*57.2958, 2)
            delta=0
            delta=int(Images[2].middleX-Images[2].contourCenterX)
            fm = RepackImages(Images)
            
            #cv2.putText(fm, "Angle: " + str(theta) , (10, 470), font, 1.5, (0,0,255), 2, cv2.LINE_AA)
            
            # cv2.imshow("frame", fm)

            print('Angle: ', theta)
            print('Distance: ', delta)
            print('Iteration: ', it)
            print('RotCon: ', rotCon)

            it = it + 1



        rospy.loginfo('publishing camera rotation')
        pub_rotation.publish(rotCon)
        
        rospy.loginfo('publishing video stream')
        pub_image.publish(br.cv2_to_imgmsg(fm, "bgr8"))

        rospy.loginfo('publishing angle')
        pub_angle.publish(theta)

        rospy.loginfo('publishing distance')
        pub_distance.publish(delta)

        if sleepCon==1:
            time.sleep(3)
            sleepCon=0
            rotCon=4
        

        rate.sleep()

if __name__ == '__main__':
    try:
        processed_image_pub()
    except rospy.ROSInterruptException:
        pass