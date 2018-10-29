import datetime

class Tracker:
    def __init__(self, detector, trackingMethodName, detectionIntervalMs):
        self.detector = detector
        self.isRunning = False
        self.__tracker = None
        self.methodName = trackingMethodName
        self.detectionIntervalMs = detectionIntervalMs
        
    def pause(self):
        self.isRunning = False
        
    def start(self):
        self.isRunning = True
    
    def _detect(self, frame):
        self.lastDetectRunTime = datetime.datetime.now()
        result = self.detector.find(frame)
        return result
    
    def update(self, frame):
        if not self.isRunning:
            return False, [], []
        
        delta = datetime.datetime.now() - self.lastDetectRunTime
        if (delta.seconds * 1000 > self.detectionIntervalMs):
            return self.init(frame)
        else:
            tracked = self._track(frame)
            
            if not tracked[0]:
                tracked = self.init(frame)
            return tracked