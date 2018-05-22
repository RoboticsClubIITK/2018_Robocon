import cv2
from imutils.video import VideoStream
from imutils.video import FPS
import time
import numpy as np

vs = VideoStream(src=1).start()#TO START STREAM FROM EXTERNAL CAMERA
time.sleep(2.0)
fps = FPS().start()

while True:
    frame = vs.read()

####################### THRESHOLD + CONTOURS #############################
    # gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    # ret, thresh = cv2.threshold(gray_frame, 127, 255, 0)
    # _, contours, _ = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    # cv2.drawContours(frame, contours, -1, (0, 255, 0), 2)
##########################################################################

####################### CANNY + POLYGON DETECTION ########################
    bilateral_filtered_image = cv2.bilateralFilter(frame, 5, 175, 175)
    #cv2.imshow('Bilateral', bilateral_filtered_image)
    #cv2.waitKey(0)

    edge_detected_image = cv2.Canny(bilateral_filtered_image, 75, 200)
    #cv2.imshow('Edge', edge_detected_image)
    #cv2.waitKey(0)

    _, contours, _ = cv2.findContours(edge_detected_image, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    contour_list = []
    for contour in contours:
        approx = cv2.approxPolyDP(contour,0.01*cv2.arcLength(contour,True),True)
        area = cv2.contourArea(contour)
        if (len(approx) >= 6 and area > 5):
            contour_list.append(contour)

    cv2.drawContours(frame, contour_list, -1, (0, 255, 0), 2)
###########################################################################

    cv2.imshow("Frame", frame)
    key = cv2.waitKey(1) & 0xFF

    if key == ord("q"):
        break

    fps.update()

# stop the timer and display FPS information
fps.stop()
