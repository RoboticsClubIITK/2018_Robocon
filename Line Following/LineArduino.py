import cv2 as cv
import numpy as np
import math
from serial import Serial
import time

#set up serial communication
# ser = Serial('/dev/ttyACM0',9600)

capture = cv.VideoCapture(1)  #read the video
capture.set(3, 320.0) #set the width
capture.set(4, 240.0)  #set the height
capture.set(5, 15)  #set the frame rate
cv.namedWindow('frame', cv.WINDOW_FULLSCREEN)

while cv.waitKey(1) & 0xff!=ord('q'):
	flag, frame = capture.read()
	# frame = cv.imread('img3.jpg')
	cv.flip(frame,0)#flip the frame
	gray=cv.cvtColor(frame,cv.COLOR_BGR2GRAY)#convert each frame to grayscale.
	blur=cv.GaussianBlur(gray,(5,5),0)#blur the grayscale image
	ret,th1 = cv.threshold(blur,35,255,cv.THRESH_BINARY+cv.THRESH_OTSU)#using threshold remove noise
	ret1,th2 = cv.threshold(th1,127,255,cv.THRESH_BINARY_INV)# invert the pixels of the image frame
	h,w = frame.shape[:2]	
	th2 = cv.erode(th2, None, iterations=2)  
	th2 = cv.dilate(th2, None, iterations=2)

	x=[0,0,0,0]
	y=[0,0,0,0]
	for i in range(0,4):
		_, contours, hierarchy = cv.findContours(th2[int((3-i)*h/4):int((4-i)*h/4),:],cv.RETR_TREE,cv.CHAIN_APPROX_SIMPLE) #find the contours
		if len(contours)>0:
			cnt=max(contours,key=cv.contourArea)
			area = cv.contourArea(cnt)# find the area of contour
			if area>=500:
				# find moment and centroid
				M = cv.moments(cnt)
				cx = int(M['m10']/M['m00'])
				cy = int((M['m01']/M['m00'])+(3-i)*h/4)
				x[i]=cx
				y[i]=cy
				for e in contours:
					cv.drawContours(frame[int((3-i)*h/4):int((4-i)*h/4), :], [e], -1, (0, 255, 255), 1)
				cv.circle(frame, (cx,cy), 5, (0, 0, 255), -1)
				cv.circle(frame, (cx,cy), 20, (255, 0, 0), 2)
	cv.imshow('frame',frame) #show video
	
	# calculate distance
	d1 = 160 - x[3]
	d2 = 160 - x[0]
	print('\n\nupper : ',d1)
	print('lower : ',d2)

	#calculate distance
	#theta = 57*math.atan((x[0]-x[3])/(y[0]-y[3]))
	#print('angle : ',theta)

	#move straight
	# if(d1<10 and d1>-10 and d2<10 and d2>-10):
	# 	ser.write(b'0')
	# #move right
	# elif(d1<-70):
	# 	ser.write(b'1')
	# 	#time.sleep(1)
	# #move left
	# elif(d1>70):
	# 	ser.write(b'2')
	# #turn right
	# #elif(d1<-50):
	# #	ser.write(b'1')
	# #turn left
	# #elif(d1>50):
	# #	ser.write(b'2')
	# else:
	# 	ser.write(b'5')
