import cv2 as cv
import numpy as np
from imutils.video import VideoStream
from imutils.video import FPS
import time
cap=VideoStream(src=1).start()
time.sleep(2.0)
fps=FPS().start()
tem=cv.imread('img.png',1)
tem=cv.resize(tem,(75,75))
w=75
h=75
cv.namedWindow('vid',cv.WINDOW_FULLSCREEN)
cv.namedWindow('match',cv.WINDOW_FULLSCREEN)
while True:
	frame=cap.read()
	if cv.waitKey(1) & 0xFF==ord('q'):
		break
	#frame=cv.cvtColor(frame,cv.COLOR_BGR2GRAY)	
	frame=cv.bilateralFilter(frame,5,75,75)
	res=cv.matchTemplate(frame,tem,cv.TM_SQDIFF_NORMED)
	threshold=0.6
	_,_,loc,_=cv.minMaxLoc(res)
	cv.rectangle(frame,loc,(loc[0]+w,loc[1]+h),(0,255,0),2)
	cv.imshow('vid',frame)
	cv.imshow('match',res)
	fps.update()
fps.stop()
