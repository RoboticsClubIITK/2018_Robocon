import cv2
from imutils.video import VideoStream
from imutils.video import FPS
import time
import numpy as np

frame = cv2.imread('img2.jpeg')
# gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
# ret, thresh = cv2.threshold(gray_frame, 127, 255, 0)

bilateral_filtered_image = cv2.bilateralFilter(frame, 5, 175, 175)
cv2.imshow('Bilateral', bilateral_filtered_image)
cv2.waitKey(0)

edge_detected_image = cv2.Canny(bilateral_filtered_image, 75, 200)
cv2.imshow('Edge', edge_detected_image)
cv2.waitKey(0)

_, contours, _ = cv2.findContours(edge_detected_image, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
# _, contours, _ = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
#
# cnt = contours[0]
# ellipse = cv2.fitEllipse(cnt)
# frame = cv2.ellipse(frame, ellipse, (0, 255, 0), 2)

#POLYGON APPROACH
contour_list = []
for contour in contours:
    approx = cv2.approxPolyDP(contour,0.01*cv2.arcLength(contour,True),True)
    if (len(approx) >= 6):
        contour_list.append(contour)

cv2.drawContours(frame, contour_list, -1, (0, 255, 0), 2)

cv2.imshow("Frame", frame)
cv2.waitKey(0)
