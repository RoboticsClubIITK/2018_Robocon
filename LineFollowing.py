import cv2 as cv
import numpy as np
capture = cv.VideoCapture(1)  #read the video
capture.set(3,320.0) #set the size
capture.set(4,240.0)  #set the size
capture.set(5,15)  #set the frame rate
cv.namedWindow('frame',cv.WINDOW_FULLSCREEN)
while cv.waitKey(1) & 0xff!=ord('q'):
    flag, frame = capture.read() #read the video in frames
    gray=cv.cvtColor(frame,cv.COLOR_BGR2GRAY)#convert each frame to grayscale.
    blur=cv.GaussianBlur(gray,(5,5),0)#blur the grayscale image
    ret,th1 = cv.threshold(blur,35,255,cv.THRESH_BINARY+cv.THRESH_OTSU)#using threshold remove noise
    ret1,th2 = cv.threshold(th1,127,255,cv.THRESH_BINARY_INV)# invert the pixels of the image frame
    th2 = cv.erode(th2, None, iterations=2)  
    th2 = cv.dilate(th2, None, iterations=2)
    h,w=frame.shape[:2]
    # im1=th2[:,int(3*h/4):int(h)]
    # im2=th2[:,int(h/2):int(3*h/4)]
    # im3=th2[:,int(h/4):int(h/2)]
    # im4=th2[:,0:int(h/4)]
    # im1=th2[int(3*h/4):int(h),:]
    # im2=th2[int(h/2):int(3*h/4),:]
    # im3=th2[int(h/4):int(h/2),:]
    # im4=th2[0:int(h/4),:]
    # im=[im1,im2,im3,im4]
    for i in range(0,4):
        _,contours, hierarchy = cv.findContours(th2[int((3-i)*h/4):int((4-i)*h/4),:],cv.RETR_TREE,cv.CHAIN_APPROX_SIMPLE) #find the contours
        if len(contours)>0:
            cnt=max(contours,key=cv.contourArea)
            area = cv.contourArea(cnt)# find the area of contour
            if area>=500:
                # find moment and centroid
                M = cv.moments(cnt)
                cx = int(M['m10']/M['m00'])
                cy = int((M['m01']/M['m00'])+(3-i)*h/4)
                for e in contours:
                    cv.drawContours(frame[int((3-i)*h/4):int((4-i)*h/4),:],[e],-1,(0,255,0),1)
                cv.circle(frame,(cx,cy),5,(0,0,255),-1)
                cv.circle(frame,(cx,cy),30,(255,0,0),2)
    cv.imshow('frame',frame) #show video 