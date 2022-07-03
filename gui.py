from kivy.uix.screenmanager import ScreenManager, Screen ,WipeTransition, FadeTransition , NoTransition , SlideTransition , FallOutTransition , RiseInTransition
from kivymd.app import MDApp
from kivy.core.window import Window
from kivy.lang.builder import Builder
from kivy.config import Config
from kivy.utils import get_color_from_hex
from kivy.uix.image import Image
from kivy.clock import Clock
from kivy.graphics.texture import Texture
import cv2




class DemoProject(ScreenManager):
    pass


class BirdApp(MDApp):

    def goto(self,screen_name):
        # saves the last screen before changing
        self.last_screen = self.root.current
        self.root.current = screen_name
    def on(self,check):
        try:
            if check == 'yes':
                self.root.ids[f'no_btn'].md_bg_color =  get_color_from_hex("#FFFFFF")
                self.root.ids[f'no_btn_label'].color = get_color_from_hex("#000000")
            elif check == 'no':
                self.root.ids[f'yes_btn'].md_bg_color =  get_color_from_hex("#FFFFFF")
                self.root.ids[f'yes_btn_label'].color = get_color_from_hex("#000000")
            elif check == 'reset':
                self.root.ids[f'yes_btn'].md_bg_color =  get_color_from_hex("#FFFFFF")
                self.root.ids[f'yes_btn_label'].color = get_color_from_hex("#000000")
                self.root.ids[f'no_btn'].md_bg_color =  get_color_from_hex("#FFFFFF")
                self.root.ids[f'no_btn_label'].color = get_color_from_hex("#000000")

            self.root.ids[f'{check}_btn'].md_bg_color =  get_color_from_hex("#012241")
            self.root.ids[f'{check}_btn_label'].color = get_color_from_hex("#FFFFFF")
        except:
            pass
    def change_text(self, id , text):
        self.root.ids[id].text = text
    def change_photo(self, id , new_path):
        self.root.ids[id].source = new_path


    def build(self):


        Builder.load_file('styles.kv')
        Builder.load_file('main.kv')

        ip = 0
        self.cap = cv2.VideoCapture(ip)
        Clock.schedule_interval(self.load_video , 1.0/30.0)

        return DemoProject()

    def load_video(self , *args):
        _, frame = self.cap.read()
        self.image_frame = frame
        buffer = cv2.flip(frame , 0 ).tobytes()
        texture = Texture.create(size=(frame.shape[1],frame.shape[0]),colorfmt='bgr')
        texture.blit_buffer(buffer, colorfmt='bgr',bufferfmt='ubyte')
        self.root.ids['camera'].texture = texture

if __name__ == '__main__':
    BirdApp().run()
