import numpy as np
import pdb
from serial import Serial
import cv2
import time
import math

from Image import *
from Utils import *

ser = Serial('/dev/ttyACM0', 4800)
font = cv2.FONT_HERSHEY_SIMPLEX
direction = 0
Images=[]
N_SLICES = 4

for q in range(N_SLICES):
    Images.append(Image())

capture = cv2.VideoCapture(0)  # read the video stream
cv2.namedWindow('frame', cv2.WINDOW_FULLSCREEN)

it = 1

while cv2.waitKey(1) & 0xff != ord('q'):
    
    flag, img = capture.read()
    # img = cv2.imread('img4.jpg')
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
        
        slope = (Images[3].x_coord - Images[0].x_coord)/180.0
        #theta = round(math.degrees(math.atan(slope)), 2)
        delta=int(Images[1].middleX-Images[1].contourCenterX)
        fm = RepackImages(Images)
        t2 = time.clock()
        cv2.putText(fm,"Time: " + str((t2-t1)*1000) + " ms", (10, 470), font, 0.5, (0,0,255), 1, cv2.LINE_AA)
        
        # for i in range(N_SLICES):
        #     cv2.imshow("part %d" % i, Images[i].image)
        #     print('error1: ', error1)
        #     print('error2: ', error2)
        #     print('error3: ', error3)
        #     print('error4: ', error4, end="\n\n")

        cv2.imshow("frame", fm)
        print('Slope: ', slope)
        print('Angle: ', delta)
        print('Iteration: ', it)

        # binary = "{0:b}".format(theta)
        
        ser.write(str.encode(str(delta) + "\n"))
        it = it + 1

        # connection.sendall( bytes(str(direction).encode('utf8')) )

cv2.destroyAllWindows()