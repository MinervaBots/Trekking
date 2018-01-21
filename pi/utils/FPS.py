# import the necessary packages
import time
import datetime

class FPS:
    def __init__(self, debug):
        # store the start time, end time, and total number of frames
        # that were examined between the start and end intervals
        self._start = None
        self._end = None
        self._last = None
        self._delta = None
        self._numFrames = 0
        self.debug = debug
        
    def start(self):
        # start the timer
        self._start = time.time()
        self._last = time.time()
        return self
 
    def stop(self, log):
        # stop the timer
        self._end = time.time()
        if(self.debug or log):
            print("[INFO] Tempo percorrido: {:.2f}".format(self.elapsed()))
            print("[INFO] FPS médio: {:.2f}".format(self.meanFps()))
     
    def update(self):
        # increment the total number of frames examined during the
        # start and end intervals
        self._numFrames += 1

        fps = 1.0 / (time.time() - self._last)
        self._last = time.time()
        
        if(self.debug):
            print("FPS: {:.2f}".format(fps))
        
        return fps
        
    def elapsed(self):
        # return the total number of seconds between the start and
        # end interval
        dateTimeStart = datetime.datetime.fromtimestamp(self._end)
        dateTimeEnd = datetime.datetime.fromtimestamp(self._start)
        return (dateTimeStart - dateTimeEnd).total_seconds()


    def meanFps(self):
        # compute the (approximate) frames per second
        return self._numFrames / self.elapsed()
