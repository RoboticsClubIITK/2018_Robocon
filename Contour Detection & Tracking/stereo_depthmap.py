import numpy as np
import cv2
from matplotlib import pyplot as plt
from imutils.video import VideoStream
from imutils.video import FPS
import time
from sklearn.preprocessing import normalize

left = VideoStream(src=1).start()
right = VideoStream(src=2).start()

window_size = 3
left_matcher = cv2.StereoSGBM_create(
minDisparity=0,
numDisparities=160,             # max_disp has to be dividable by 16 f. E. HH 192, 256
blockSize=5,
P1=8 * 3 * window_size ** 2,    # wsize default 3; 5; 7 for SGBM reduced size image; 15 for SGBM full size image (1300px and above); 5 Works nicely
P2=32 * 3 * window_size ** 2,
disp12MaxDiff=1,
uniquenessRatio=15,
speckleWindowSize=0,
speckleRange=2,
preFilterCap=63,
mode=cv2.STEREO_SGBM_MODE_SGBM_3WAY
)
right_matcher = cv2.ximgproc.createRightMatcher(left_matcher)

lmbda = 80000
sigma = 1.2
visual_multiplier = 1.0

wls_filter = cv2.ximgproc.createDisparityWLSFilter(matcher_left=left_matcher)
wls_filter.setLambda(lmbda)
wls_filter.setSigmaColor(sigma)

while True:
    imgL = left.read()
    imgR = right.read()

    #print('computing disparity...')
    displ = left_matcher.compute(imgL, imgR)  # .astype(np.float32)/16
    dispr = right_matcher.compute(imgR, imgL)  # .astype(np.float32)/16
    displ = np.int16(displ)
    dispr = np.int16(dispr)
    filteredImg = wls_filter.filter(displ, imgL, None, dispr)

    filteredImg = cv2.normalize(src=filteredImg, dst=filteredImg, beta=0, alpha=255, norm_type=cv2.NORM_MINMAX);
    filteredImg = np.uint8(filteredImg)
    cv2.imshow('Disparity Map', filteredImg)
    #cv2.waitKey()
cv2.destroyAllWindows()

# cv2.namedWindow('Left', cv2.WINDOW_FULLSCREEN)
# cv2.namedWindow('Right', cv2.WINDOW_FULLSCREEN)
# cv2.namedWindow('Depth', cv2.WINDOW_FULLSCREEN)
# stereo = cv2.StereoBM_create(numDisparities=16, blockSize=15)

# time.sleep(2.0)
# fps = FPS().start()

# while True:
#     if cv2.waitKey(1) & 0xff==ord('q'):
#         break
    
#     left_frame = left.read()
#     right_frame = right.read()
#     left_thresh = cv2.cvtColor(left_frame, cv2.COLOR_BGR2GRAY)
#     right_thresh = cv2.cvtColor(right_frame, cv2.COLOR_BGR2GRAY)

#     disparity = stereo.compute(left_thresh, right_thresh)

#     cv2.imshow('Left', left_frame)
#     cv2.imshow('Right', right_frame)
#     cv2.imshow('Depth', disparity)

#     plt.imshow(disparity,'gray')

cv2.destroyAllWindows()