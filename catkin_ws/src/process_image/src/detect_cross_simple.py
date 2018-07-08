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


# capture = cv2.VideoCapture(1)

# while cv2.waitKey(1) & 0xff != ord('q'):
#     ret, image = capture.read()

def detect_cross(image):
    # image = cv2.imread('./cross.jpg')
    blur=cv2.GaussianBlur(image,(11,11),0)#blur the grayscale image
    hsv=cv2.cvtColor(blur,cv2.COLOR_BGR2HSV)#convert each frame to grayscale.
    mask=cv2.inRange(hsv,low,upp)

    #ret,th1 = cv2.threshold(mask,100,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)#using threshold remove noise
    #ret1,th2 = cv2.threshold(th1,100,255,cv2.THRESH_BINARY_INV)# invert the pixels of the image frame

    thresh = cv2.erode(mask, None, iterations=2)  
    thresh = cv2.dilate(thresh, None, iterations=2)

# gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    edges = cv2.Canny(thresh, 50, 150, apertureSize=3)
    lines = cv2.HoughLines(edges, 1, np.pi/180, 50)

    # vertical_line = []
    # vertical_num = 0

    # horizontal_line = []
    # horizontal_num = 0

    # for line in lines:
    #     rho, theta = line[0]

    #     if (theta>175*np.pi/180.0 and theta<180*np.pi/180.0) or (theta<5*np.pi/180.0 and theta>0):
    #         horizontal_num+=1
    #         horizontal_line.append(line)
    #     elif theta>85*np.pi/180.0 and theta<95*np.pi/180.0:
    #         vertical_num += 1
    #         vertical_line.append(line)

    # minhoriz = 10000000
    # for line in horizontal_line:
    #     rho, theta = line[0]

    #     rho = abs(rho)
    #     if rho < minhoriz:
    #         minhoriz = rho
    #         minline = line
    
    # rho, theta = minline[0]
    # rho = abs(rho)
    # a = np.cos(theta)
    # b = np.sin(theta)
    # x0 = a*rho
    # y0 = b*rho
    # x1 = int(x0+1000*(-b))
    # y1 = int(y0+1000*a)
    # x2 = int(x0-1000*(-b))
    # y2 = int(y0-1000*a)
    # cv2.line(image, (x1,y1), (x2,y2),(0,0,255),2)

    slopearr=[]
    flag = 0

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


        if flag == 1:
            for angle in slopearr:
                if(abs(theta-angle) > 85*np.pi/180.0 and abs(theta-angle)<95*np.pi/180.0):
                    line2 = line
                    cv2.line(image, (x1,y1), (x2,y2),(0,0,255),2)
                    flag = 2
                    break
        elif flag == 0:
            flag = 1
            line1 = line
            cv2.line(image, (x1,y1), (x2,y2),(0,0,255),2)
            slopearr.append(theta)
        elif flag == 2:
            break
    if flag == 2:
        [[x, y]] = intersection(line1, line2)
        cv2.circle(image, (x, y), 3, 255, -1)
        return 1
    elif flag == 1:
        return 0

    cv2.imshow('img', image)