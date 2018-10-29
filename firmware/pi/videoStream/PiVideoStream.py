# import the necessary packages
from picamera.array import PiRGBArray
from picamera import PiCamera
from threading import Thread
import cv2
 
class PiVideoStream:
    def __init__(self, resolution=(320, 240), framerate=32):
        # initialize the camera and stream
        self.camera = PiCamera()
        self.camera.resolution = resolution
        self.camera.framerate = framerate
        self.rawCapture = PiRGBArray(self.camera, size=resolution)

        # initialize the frame and the variable used to indicate
        # if the thread should be stopped
        self.frame = None
        self.stopped = False

    @property
    def resolution(self):
        return self.camera.resolution
        
    def start(self):
        self.stream = self.camera.capture_continuous(self.rawCapture, format="bgr", use_video_port=True)
        # start the thread to read frames from the video stream
        
        self.__thread = Thread(target=self.update, args=())
        self.__thread.daemon = True
        self.__thread.start()
        
        return self
 
    def update(self):
        # keep looping infinitely until the thread is stopped
        for f in self.stream:
        
            # if the thread indicator variable is set, stop the thread
            # and resource camera resources
            if self.stopped:
                break
                
            # grab the frame from the stream and clear the stream in
            # preparation for the next frame
            self.frame = f.array
            self.rawCapture.truncate(0)
     
                
        self.stream.close()
        self.rawCapture.close()
        self.camera.close()
        
    def read(self):
        # return the frame most recently read
        return self.frame
 
    def stop(self):
        # indicate that the thread should be stopped
        self.stopped = True
        self.__thread.join()