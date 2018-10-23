import time
import datetime

class PerfCounter:
    def __init__(self, debug):
        # store the start time, end time, and total number of frames
        # that were examined between the start and end intervals
        self._start = None
        self._end = None
        self._last = None
        self._delta = None
        self._numFrames = 0
        self.debug = debug
        self.__lastMsFrame = 0
        self.__lastFps = 0
        self.__totalFrames = 0
        
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
            print("[INFO] FPS médio: {:.0f}".format(self.meanFps()))
            print("[INFO] ms/frame médio: {:.2f}".format(self.meanMsFrame()))
     
    def update(self):
        # increment the total number of frames examined during the
        # start and end intervals
        self._numFrames += 1
        
        dif = (time.time() - self._last)
        if (dif > 0):
            self.__lastMsFrame = dif * 1000.0 / self._numFrames
            self.__lastFps = self._numFrames / dif
            
            self.__totalFrames += self._numFrames
            self._numFrames = 0
            self._last = time.time()
        
        if(self.debug):
            print("FPS: {:.0f} \t ms/frame: {:.2f}".format(self.__lastFps, self.__lastMsFrame))
        
        return self.__lastFps, self.__lastMsFrame
        
    def elapsed(self):
        # return the total number of seconds between the start and
        # end interval
        return (self._end - self._start)

    def meanMsFrame(self):
        return self.elapsed() * 1000.0 / self.__totalFrames

    def meanFps(self):
        # compute the (approximate) frames per second
        return self.__totalFrames / self.elapsed()
