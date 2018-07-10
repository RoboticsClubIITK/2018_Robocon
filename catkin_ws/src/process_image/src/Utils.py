import numpy as np
import cv2
import time
from Image_process import *

low=(50,20,5)
upp=(150,150,150)

def SlicePart(im, images, slices):
    height, width = im.shape[:2]
    sl = int(height/slices)
    
    for i in range(slices):
        part = sl*i
        crop_img = im[part:part+sl, 0:width]
        images[i].image = crop_img
        images[i].Process()
    
def RepackImages(images):
    img = images[0].image
    for i in range(len(images)):
        if i == 0:
            img = np.concatenate((img, images[1].image), axis=0)
        if i > 1:
            img = np.concatenate((img, images[i].image), axis=0)
            
    return img

def Center(moments):
    if moments["m00"] == 0:
        return 0
        
    x = int(moments["m10"]/moments["m00"])
    y = int(moments["m01"]/moments["m00"])

    return x, y
    
def RemoveBackground(image, b):
    up = 130
    # create NumPy arrays from the boundaries
    lower = np.array([0, 0, 0], dtype = "uint8")
    upper = np.array([up, up, up], dtype = "uint8")
    #----------------COLOR SELECTION-------------- (Remove any area that is whiter than 'upper')
    if b == True:
        mask = cv2.inRange(image, lower, upper)
        image = cv2.bitwise_and(image, image, mask = mask)
        image = cv2.bitwise_not(image, image, mask = mask)
        image = (255-image)
        return image
    else:
        return image

def filter(image):
    blur=cv2.GaussianBlur(image,(11,11),0)#blur the grayscale image
    hsv=cv2.cvtColor(blur,cv2.COLOR_BGR2HSV)#convert each frame to grayscale.
    mask=cv2.inRange(hsv,low,upp)
    #ret,th1 = cv2.threshold(mask,100,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)#using threshold remove noise
    #ret1,th2 = cv2.threshold(th1,100,255,cv2.THRESH_BINARY_INV)# invert the pixels of the image frame
    thresh = cv2.erode(mask, None, iterations=2)  
    thresh = cv2.dilate(thresh, None, iterations=2)
    return thresh