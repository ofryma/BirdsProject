import cv2
import numpy as np
import tensorflow as tf
from pyautogui import *
import pyautogui
import time
import webbrowser
from tensorflow.keras.preprocessing import image
from tensorflow.keras.applications import imagenet_utils
import requests


from gui import changeWidth , changeHeight




## ------ Screen use interface ------

def changeWidth(value):
    global x
    print('xPos: ',value)
    x=value
def changeHeight(value):
    global y
    print('yPos',value)
    y=value


textPosition = (30,50)
rectangleBL= (25,55)
rectangleTR=(100,35)
textSize=0.5
textFont=cv2.FONT_HERSHEY_COMPLEX
textColor=[0,0,255]

width,height = pyautogui.size()

trackBarWindow = 'myTrackar'
cv2.namedWindow(trackBarWindow)
cv2.resizeWindow(trackBarWindow,500,200)
cv2.createTrackbar('xPos',trackBarWindow,0,415,changeWidth)
cv2.createTrackbar('yPos',trackBarWindow,0,250,changeHeight)

## ---------------------------------


## deep learning model with weights - pre trained:
mobile = tf.keras.applications.mobilenet.MobileNet()


# Size of frame for deep learning model
box_width=224
box_height=224
x = 0
y= 0
red = (0,0,255)
green = (0,255,0)

# IP webcam of the galaxy phone
ip = 'http://172.20.10.6:8080/video'
ip = 0
cap = cv2.VideoCapture(ip)



while True:
    _, frame = cap.read()


    frame1 = frame[y:y+box_height, x:x+box_width] # bottom right
    cv2.rectangle(frame, (x,y), (x + box_width, y + box_height), red, 3)

    resized_img = image.img_to_array(frame1)
    final_image = np.expand_dims(resized_img, axis=0)  ## need fourth dimention
    final_image = tf.keras.applications.mobilenet.preprocess_input(final_image)

    predictions = mobile.predict(final_image)
    results = imagenet_utils.decode_predictions(predictions, top=3)



    if ('n02877765' == results[0][0][0] or 'n02877765' == results[0][1][0] or 'n02877765' == results[0][2][0]):
        txt = 'bottlecap'
        textColor = green
        # print('bottlecap')
        c = 1
        cv2.rectangle(frame, (x,y), (x + box_width, y + box_height), green, 3)


    else:
        txt = '---------'
        textColor = red
        # print('---------')
        c = 0



    cv2.putText(frame, txt, (textPosition[0] + x,textPosition[1]+y), textFont, textSize, textColor)
    cv2.imshow("Camera", frame)

    if cv2.waitKey(1) == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()





