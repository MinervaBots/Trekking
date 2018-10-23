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
    
    def _shouldRunDetection(self):
        delta = datetime.datetime.now() - self.lastDetectRunTime
        return delta.seconds * 1000 > self.detectionIntervalMs
    
    def _detect(self, frame):
        self.lastDetectRunTime = datetime.datetime.now()
        return self.detector.find(frame)
    
    def update(self, frame):
        if not self.isRunning:
            return False, None, 0
            
        if (self._shouldRunDetection()):
            return self._detect(frame)
        
        return self._track(frame)