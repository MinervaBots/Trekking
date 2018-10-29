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
            return False, None, 0
        
        delta = datetime.datetime.now() - self.lastDetectRunTime
        tracked = self._track(frame)
        if not tracked[0] or delta.seconds * 1000 > self.detectionIntervalMs:
            return self._detect(frame)
        return tracked