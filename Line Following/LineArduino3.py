import numpy as np
import cv2
from serial import Serial
import math
import time

def getContourCenter(contour):
    M = cv2.moments(contour)

    if M["m00"] == 0:
        return 0

    x = int(M["m10"]/M["m00"])
    y = int(M["m01"]/M["m00"])

    return [x, y]

def getContourExtent(contour):
    area = cv2.contourArea(contour)
    x, y, w, h = cv2.boundingRect(contour)
    rect_area = w*h
    if rect_area > 0:
        return (float(area)/rect_area)

def Aprox(a, b, error):
    if abs(a - b) < error:
        return True
    else:
        return False

def correctMainContour(prev_cx, contourCenterX):
    if abs(prev_cx-contourCenterX) > 5:
        for i in range(len(contours)):
            if getContourCenter(contours[i]) != 0:
                tmp_cx = getContourCenter(contours[i])[0]
                if Aprox(tmp_cx, prev_cx, 5) == True:
                    MainContour = contours[i]
                    if getContourCenter(MainContour) != 0:
                        contourCenterX = getContourCenter(MainContour)[0]

ser = Serial('/dev/ttyACM0', 9600)

capture = cv2.VideoCapture(1)  # read the video stream
capture.set(3, 320.0)  # set the width
capture.set(4, 240.0)  # set the height
capture.set(5, 15)  # set the frame rate
cv2.namedWindow('frame', cv2.WINDOW_FULLSCREEN)

while cv2.waitKey(1) & 0xff != ord('q'):
    flag, image = capture.read()  # read the video in frames
    contourCenterX = 0
    imgray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY) #Convert to Gray Scale
    ret, thresh = cv2.threshold(imgray, 100, 255, cv2.THRESH_BINARY_INV) #Get Threshold
    _, contours, _ = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE) #Get contour

    # prev_MC = MainContour
    if contours:
        MainContour = max(contours, key=cv2.contourArea)

        height, width  = image.shape[:2]

        middleX = int(width/2) #Get X coordinate of the middle point
        middleY = int(height/2) #Get Y coordinate of the middle point
        
        prev_cX = contourCenterX
        if getContourCenter(MainContour) != 0:
            contourCenterX = getContourCenter(MainContour)[0]
            if abs(prev_cX-contourCenterX) > 5:
                correctMainContour(prev_cX, contourCenterX)
        else:
            contourCenterX = 0
        
        dir =  int((middleX-contourCenterX) * getContourExtent(MainContour))
        
        cv2.drawContours(image, MainContour,-1, (0, 255, 0), 3) #Draw Contour GREEN
        cv2.circle(image, (contourCenterX, middleY), 7, (255, 255, 255), -1) #Draw dX circle WHITE
        cv2.circle(image, (middleX, middleY), 3, (0, 0, 255), -1) #Draw middle circle RED
        
        font = cv2.FONT_HERSHEY_SIMPLEX
        cv2.putText(image, str(middleX-contourCenterX), (contourCenterX+20, middleY), font, 1, (200,0,200), 2, cv2.LINE_AA)
        cv2.putText(image, "Weight:%.3f" % getContourExtent(MainContour), (contourCenterX+20, middleY+35), font, 0.5, (200,0,200), 1, cv2.LINE_AA)

        cv2.imshow('image', image)

