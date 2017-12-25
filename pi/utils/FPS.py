# import the necessary packages
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
        self._start = datetime.datetime.now()
        self._last = datetime.datetime.now()
        return self
 
    def stop(self):
        # stop the timer
        self._end = datetime.datetime.now()
        if(self.debug):
            print("[INFO] Tempo percorrido: {:.2f}".format(self.elapsed()))
            print("[INFO] FPS médio: {:.2f}".format(self.meanFps()))
     
    def update(self):
        # increment the total number of frames examined during the
        # start and end intervals
        self._numFrames += 1
        self._delta = datetime.datetime.now() - self._last
        self._last = datetime.datetime.now()
        if(self.debug):
            print("FPS: {:.2f}".format(self.fps()))
        
    def deltaTime(self):
        return self._delta.total_seconds()
    
    def elapsed(self):
        # return the total number of seconds between the start and
        # end interval
        return (self._end - self._start).total_seconds()  

    def fps(self):
        return 1.0 / self._delta.total_seconds()  
        
    def meanFps(self):
        # compute the (approximate) frames per second
        return self._numFrames / self.elapsed()
