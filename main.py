# Uncomment these lines to see all the messages
# from kivy.logger import Logger
# import logging
# Logger.setLevel(logging.TRACE)


from kivy.uix.screenmanager import ScreenManager
from kivy.utils import get_color_from_hex
from kivy.app import App
from kivy.lang import Builder
from kivy.uix.boxlayout import BoxLayout
from kivy.clock import Clock
from kivy.graphics.texture import Texture
import time

import numpy as np
# import pyautogui

import tensorflow as tf
from tensorflow.keras.preprocessing import image
from tensorflow.keras.applications import imagenet_utils
import cv2


# deep learning model with weights - pre trained:
mobile = tf.keras.applications.mobilenet.MobileNet()


# Size of frame for deep learning model
box_width=224
box_height=224
x = 0
y= 0
blue = (255,0,0)
green = (0,255,0)
red = (0,0,255)



class DemoProject(ScreenManager):
    pass



class BirdApp(App):
    def build(self):
        self.cap = cv2.VideoCapture(0)
        Clock.schedule_interval(self.load_video , 1.0 / 30.0)
        Builder.load_file('camera.kv')

        return DemoProject()

    def load_video(self , *args):

        '''
        This function creating a prediction using the video captured
        if there is a bottlecap in the image, this function will call
        the send_prediction function with the results
        '''

        _, frame = self.cap.read()
        self.image_frame = frame

        frame1 = frame[y:y + box_height, x:x + box_width]  # bottom right
        cv2.rectangle(frame, (x, y), (x + box_width, y + box_height), blue, 3)


        resized_img = image.img_to_array(frame1)
        final_image = np.expand_dims(resized_img, axis=0)  ## need fourth dimention
        final_image = tf.keras.applications.mobilenet.preprocess_input(final_image)

        # Creating a prediction
        check_places = 3
        predictions = mobile.predict(final_image)
        results = imagenet_utils.decode_predictions(predictions, top=check_places)



        bottlecap = 'n02877765'
        res = []

        for i in range(0,check_places):
            res.append(results[0][i][0])
        if(bottlecap in res):
            txt = 'bottlecap'
            textColor = get_color_from_hex('#1BAD1A')
            c = 1
        else:
            txt = '---------'
            textColor = get_color_from_hex('#000000')
            c = 0

        self.root.ids['prediction'].text = txt
        self.root.ids['prediction'].color = textColor
        buffer = cv2.flip(frame , 0 ).tobytes()
        texture = Texture.create(size=(frame.shape[1],frame.shape[0]),colorfmt='bgr')
        texture.blit_buffer(buffer, colorfmt='bgr',bufferfmt='ubyte')
        self.root.ids['camera'].texture = texture


    def send_prediction(self):
        '''
        This funtion is sending the prediction to the arduino
        that controls the mechanism via http request over the
        wifi network
        '''
        pass


if __name__ == '__main__':
    BirdApp().run()
