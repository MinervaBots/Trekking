import cv2
import datetime
from .Tracker import Tracker
 
class OpenCVTracker(Tracker):
    def __init__(self, detector, trackingMethodName = None):
        Tracker.__init__(self, detector)
        self.__tracker = None
        self.usingTracker = False
        self.setTrackingMethod(trackingMethodName)
        self.isRunning = False
        
    def setTrackingMethod(self, trackingMethodName):
        self.methodName = trackingMethodName
        self.__selectTracker()

    def pause(self):
        self.isRunning = False
        
    def start(self):
        self.isRunning = True
        
    def init(self, frame):
        if not self.isRunning:
            return False, None, 0
            
        detected, boundingBox, direction = self.__detect(frame)

        if self.usingTracker and detected:
            self.clear()
            boundingBox = tuple(boundingBox)
            return self.__tracker.init(frame, boundingBox), boundingBox, self.detector.rectToDirection(boundingBox)
            
        return detected, boundingBox, direction
    
    def update(self, frame):
        if not self.isRunning:
            return False, None, 0
            
        if self.usingTracker:
            delta = datetime.datetime.now() - self.lastDetectRunTime
            if delta.seconds > 2:
                return self.init(frame)

            found, boundingBox = self.__tracker.update(frame)
            return found, boundingBox, self.detector.rectToDirection(boundingBox)
        else:
            return self.__detect(frame)
    
    def __detect(self, frame):
        self.lastDetectRunTime = datetime.datetime.now()
        return self.detector.find(frame)
        
    def clear(self):
        if(self.__tracker is not None):
            self.__tracker.clear()
        self.__selectTracker()

    def __selectTracker(self):
        if self.methodName == '' or self.methodName == 'CASCADE' or self.methodName == None:
            self.__tracker = None
        elif self.methodName == 'BOOSTING':
            self.__tracker = cv2.TrackerBoosting_create()
        elif self.methodName == 'MIL':
            self.__tracker = cv2.TrackerMIL_create()
        elif self.methodName == 'KCF':
            self.__tracker = cv2.TrackerKCF_create()
        elif self.methodName == 'TLD':
            self.__tracker = cv2.TrackerTLD_create()
        elif self.methodName == 'MEDIANFLOW':
            self.__tracker = cv2.TrackerMedianFlow_create()
        elif self.methodName == 'GOTURN':
            self.__tracker = cv2.TrackerGOTURN_create()
        elif self.methodName == 'MOSSE':
            self.__tracker = cv2.TrackerMOSSE_create()
        else:
            print("Algortimo '" + self.methodName + "' não reconhecido")
            self.methodName = ""
            
        self.usingTracker = not(self.__tracker is None)
