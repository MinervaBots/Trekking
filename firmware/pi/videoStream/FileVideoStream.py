# import the necessary packages
from threading import Thread
import sys
import cv2
import time
from .VideoStream import VideoStream

# import the Queue class from Python 3
if sys.version_info >= (3, 0):
    from queue import Queue
# otherwise, import the Queue class for Python 2.7
else:
    from Queue import Queue

class FileVideoStream(VideoStream):
    def __init__(self, path, resolution, queueSize=128):
        VideoStream.__init__(self, resolution = resolution)
        # initialize the file video stream along with the boolean
        # used to indicate if the thread should be stopped or not
        self.stream = cv2.VideoCapture(path)
        self.stopped = False

        # initialize the queue used to store frames read from
        # the video file
        self.Q = Queue(maxsize=queueSize)


    def start(self):
        # start a thread to read frames from the file video stream
        self.__thread = Thread(target=self.update, args=())
        self.__thread.daemon = True
        self.__thread.start()
        
        return self

    def update(self):
        # keep looping infinitely
        while True:
            # if the thread indicator variable is set, stop the
            # thread
            if self.stopped:
                break
     
            # otherwise, ensure the queue has room in it
            if not self.Q.full():
                # read the next frame from the file
                (grabbed, frame) = self.stream.read()
     
                # if the `grabbed` boolean is `False`, then we have
                # reached the end of the video file
                if not grabbed:
                    self.stop()
                    break
     
                # add the frame to the queue
                self.Q.put(frame)
            else:
                time.sleep(0.01)

        self.stream.release()
                
    def running(self):
        return self.more() or not self.stopped

    def read(self):
        # return next frame in the queue
        return self.Q.get()

    def more(self):
        # return True if there are still frames in the queue. If stream is not stopped, try to wait a moment
        tries = 0
        while self.Q.qsize() == 0 and not self.stopped and tries < 5:
            time.sleep(0.1)
            tries += 1
            
        return self.Q.qsize() > 0

    def stop(self):
        # indicate that the thread should be stopped
        self.stopped = True
        # wait until stream resources are released (producer thread might be still grabbing frame)
        self.__thread.join()