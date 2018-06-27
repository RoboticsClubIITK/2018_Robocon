import cv2 as cv 
import numpy as np
low=(70,80,50)
upp=(150,150,150)
cap=cv.VideoCapture(3)
cv.namedWindow('Frame',cv.WINDOW_FULLSCREEN)
cv.namedWindow('Mask',cv.WINDOW_FULLSCREEN)
while True:
    ret,frame=cap.read()
    if cv.waitKey(1) & 0xff==ord('q'):
        break
    eframe=cv.GaussianBlur(frame,(11,11),0)
    eframe=cv.cvtColor(eframe,cv.COLOR_BGR2HSV)
    mask=cv.inRange(eframe,low,upp)
    mask=cv.erode(mask,None,iterations=2)
    mask=cv.dilate(mask,None,iterations=2)
    cntr=cv.findContours(mask.copy(),cv.RETR_EXTERNAL,cv.CHAIN_APPROX_SIMPLE)[-2]
    if len(cntr)>0:
        c=max(cntr,key=cv.contourArea)
        for e in cntr:
            cv.drawContours(frame,[e],-1,(255,0,0),1)
    cv.imshow('Frame',frame)
    cv.imshow('Mask',mask)
cv.destroyAllWindows()