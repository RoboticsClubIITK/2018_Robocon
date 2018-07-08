import numpy as np
import cv2

low=(50,20,5)
upp=(150,150,150)
def intersection(line1, line2):
    """See https://stackoverflow.com/a/383527/5087436"""

    rho1, theta1 = line1[0]
    rho2, theta2 = line2[0]
    A = np.array([
        [np.cos(theta1), np.sin(theta1)],
        [np.cos(theta2), np.sin(theta2)]
    ])
    b = np.array([[rho1], [rho2]])
    x0, y0 = np.linalg.solve(A, b)
    x0, y0 = int(np.round(x0)), int(np.round(y0))
    return [[x0, y0]]


capture = cv2.VideoCapture(1)

while cv2.waitKey(1) & 0xff != ord('q'):
    ret, image = capture.read()

    # image = cv2.imread('./cross.jpg')
    blur=cv2.GaussianBlur(image,(11,11),0)#blur the grayscale image
    hsv=cv2.cvtColor(blur,cv2.COLOR_BGR2HSV)#convert each frame to grayscale.
    mask=cv2.inRange(hsv,low,upp)

    #ret,th1 = cv2.threshold(mask,100,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)#using threshold remove noise
    #ret1,th2 = cv2.threshold(th1,100,255,cv2.THRESH_BINARY_INV)# invert the pixels of the image frame

    thresh = cv2.erode(mask, None, iterations=2)  
    thresh = cv2.dilate(thresh, None, iterations=2)

    # gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # corners = cv2.goodFeaturesToTrack(thresh, 5, 0.5, 10)

    # for i in corners:
    #     x, y = i.ravel()
    #     cv2.circle(image, (x, y), 3, 255, -1)

    edges = cv2.Canny(thresh, 50, 150, apertureSize=3)
    lines = cv2.HoughLines(edges, 1, np.pi/180, 50)

    vertical_rho = []
    vertical_num = 0

    horizontal_rho = []
    horizontal_num = 0

    for line in lines:
        rho, theta = line[0]
        a = np.cos(theta)
        b = np.sin(theta)
        x0 = a*rho
        y0 = b*rho
        x1 = int(x0+1000*(-b))
        y1 = int(y0+1000*a)
        x2 = int(x0-1000*(-b))
        y2 = int(y0-1000*a)

        if (theta>175*np.pi/180.0 and theta<180*np.pi/180.0) or (theta<5*np.pi/180.0 and theta>0):
            horizontal_num+=1
            horizontal_rho.append(abs(rho))
            
            cv2.line(image, (x1,y1), (x2,y2),(0,0,255),2)
            # print("Theta: %.2f  rho: %.2f" % (theta*180/np.pi, horizontal_rho))

        elif theta>85*np.pi/180.0 and theta<95*np.pi/180.0:
            vertical_num += 1
            vertical_rho.append(rho)
            cv2.line(image, (x1,y1), (x2,y2),(0,255,0),2)

    if 
    h_rho = np.mean(horizontal_rho)
    v_rho = np.mean(vertical_rho)

    line1 = [[h_rho, 0]]
    line2 = [[v_rho, 90*np.pi/180.0]]

    [[x, y]] = intersection(line1, line2)
    cv2.circle(image, (x, y), 3, 255, -1)

    cv2.imshow('img', image)