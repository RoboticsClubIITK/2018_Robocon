from collections import deque
import numpy as np
import argparse
import imutils
import cv2


ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video",
    help="path to the (optional) video file")
ap.add_argument("-b", "--buffer", type=int, default=64,
    help="max buffer size")
args = vars(ap.parse_args())


Lower = (10, 80, 80)
Upper = (50, 255, 255)
pts = deque(maxlen=args["buffer"])

if not args.get("video", False):
    camera = cv2.VideoCapture(0)


else:
    camera = cv2.VideoCapture(args["video"])


while True:
    
    (grabbed, frame) = camera.read()

    if args.get("video") and not grabbed:
        break

    frame = imutils.resize(frame, width=600)
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    cv2.imshow("hsv",hsv)

    mask = cv2.inRange(hsv, Lower, Upper)
    mask = cv2.erode(mask, None, iterations=2) #narrow
    mask = cv2.dilate(mask, None, iterations=2) #widen

    cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
        cv2.CHAIN_APPROX_SIMPLE)[-2]
    center = None

    if len(cnts) > 0:
        c = max(cnts, key=cv2.contourArea)
        d=cv2.contourArea(c)
        print(d)
        for e in cnts:
            cv2.drawContours(frame, [e], -1, (0, 255, 0), 2)
        ((x, y), radius) = cv2.minEnclosingCircle(c)
        M = cv2.moments(c)
        center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))

        if radius > 10:
            cv2.circle(frame, (int(x), int(y)), int(radius),
                (0, 255, 255), 2)
            cv2.circle(frame, center, 5, (0, 0, 255), -1)

    pts.appendleft(center)

    for i in range(1, len(pts)):
        if pts[i - 1] is None or pts[i] is None:
            continue

        thickness = int(np.sqrt(args["buffer"] / float(i + 1)) * 2.5)
        cv2.line(frame, pts[i - 1], pts[i], (0,255 , 0), thickness)
    
    frame = cv2.GaussianBlur(frame, (3, 3), 0)
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    lower_blue = np.array([160,70,70])
    upper_blue = np.array([180,255,255])

    mask = cv2.inRange(hsv, lower_blue, upper_blue)
    res = cv2.bitwise_and(frame,frame, mask= mask)
    res = cv2.dilate(res, None, iterations=2)
    res=cv2.cvtColor(res,cv2.COLOR_BGR2GRAY)
    circles = cv2.HoughCircles(res, cv2.HOUGH_GRADIENT, 1.7, 50)

    if circles is not None:
        circles = np.round(circles[0, :]).astype("int")
        p=len(circles)  
        sx,sy,sz=0,0,0
        for (x, y, r) in circles:
            sx+=x
            sy+=y
            sz+=r
        x=(int)(sx/p)
        y=(int)(sy/p)
        r=(int)(sz/p)
        cv2.circle(frame, (x, y), r, (0, 255, 0), 4)
        cv2.rectangle(frame, (x - 5, y - 5), (x + 5, y + 5), (0, 128, 255), -1)
    cv2.imshow("img1",res)  
    cv2.imshow('frame',frame)
    key = cv2.waitKey(1) & 0xFF

    if key == ord("q"):
        break

camera.release()
cv2.destroyAllWindows()
