import numpy as np
import cv2
low=(50,20,5)
upp=(150,150,150)

def intersection(line1, line2):

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


def detectIntersect(image):
    # corners = cv2.goodFeaturesToTrack(gray, 25, 0.01, 10)

    # for i in corners:
    #     x, y = i.ravel()
    #     cv2.circle(image, (x, y), 3, 255, -1)

    edges = cv2.Canny(image, 50, 150, apertureSize=3)
    lines = cv2.HoughLines(edges, 1, np.pi/180, 50)

    slopearr=[]
    flag = 0
    if lines is None:
        return 0
    else:
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