import cv2 as cv 
import numpy as np 
from imutils.video import VideoStream
import imutils
pinkl=(150,50,100)
pinkh=(175,255,255)
cap=VideoStream(src=1).start()
cv.namedWindow('Ball',cv.WINDOW_FULLSCREEN)
cv.namedWindow('Mask',cv.WINDOW_FULLSCREEN)
while True:
    frame=cap.read()
    frame = imutils.resize(frame, width=600)
    if cv.waitKey(1) & 0xff==ord('q'):
        break
    eframe=cv.GaussianBlur(frame,(11,11),0)
    eframe=cv.cvtColor(eframe,cv.COLOR_BGR2HSV)
    mask=cv.inRange(eframe,pinkl,pinkh)
    mask=cv.erode(mask,None,iterations=2)
    mask=cv.dilate(mask,None,iterations=2)
    cntr=cv.findContours(mask.copy(),cv.RETR_EXTERNAL,cv.CHAIN_APPROX_SIMPLE)[-2]
    if len(cntr)>0:
        c=max(cntr,key=cv.contourArea)
        for e in cntr:
            cv.drawContours(frame,[e],-1,(255,0,0),1)
        ((x,y),radius)=cv.minEnclosingCircle(c)
        center=(int(x),int(y))
        radius=int(radius)
        M=cv.moments(c)
        centroid=(int(M["m10"]/M["m00"]),int(M["m01"]/M["m00"]))
        if radius>5:
            cv.circle(frame,center,radius,(0,255,0),2)
            cv.circle(frame,centroid,5,(0,0,255),-1)
            dist=5.033512 + (611.8415-5.033512)/(1 + pow((radius/12.93463),1.196176))
            print(int(x),"\t",int(y),"\t",int(dist))
    cv.imshow('Ball',frame)
    cv.imshow('Mask',mask)
cv.destroyAllWindows()