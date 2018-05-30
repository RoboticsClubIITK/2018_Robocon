import cv2 as cv 
import numpy as np 
from imutils.video import VideoStream
pinkl=(280,30,60)
pinkh=(360,85,100)
cap=VideoStream(src=1).start()
img=cv.imread('img.png',1)
img=cv.resize(img,(75,75))
cv.namedWindow('Ball',cv.WINDOW_FULLSCREEN)
while True:
    frame=cap.read()
    if cv.waitKey(1) & 0xff==ord('q'):
        break
    eframe=cv.bilateralFilter(frame,5,75,75)
    eframe=cv.cvtColor(eframe,cv.COLOR_BGR2HSV)
    mask=cv.inRange(eframe,pinkl,pinkh)
    mask=cv.erode(mask,None,2)
    mask=cv.dilate(mask,None,2)
    _,cntr,_=cv.findContours(mask,cv.RETR_TREE,cv.CHAIN_APPROX_SIMPLE)
    if len(cntr)>0:
        c=max(cntr,cv.contourArea)
        (x,y),radius=cv.minEnclosingCircle(c)
        center=(int(x),int(y))
        radius=int(radius)
        if radius>10:
            cv.circle(frame,center,radius,(0,255,0),2)
    cv.imshow('Ball',frame)
cv.destroyAllWindows()