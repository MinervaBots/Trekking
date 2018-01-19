import cv2
import datetime

class Tracker:
    def __init__(self, cascadePath, trackingMethodName = ""):
        self._tracker = None
        self.usingTracker = False
        self._cascadeDetector = cv2.CascadeClassifier(cascadePath)
        self.setTrackerMethod(trackingMethodName)
        
    def setTrackerMethod(self, trackingMethodName):
        self.methodName = trackingMethodName
        self._selectTracker()
        
    def init(self, frame, minSize = (0,0), maxSize = (0,0), colorLower = None, colorUpper = None, minPresence = 0.0):
        detected, boundingBox = self._detect(frame, minSize, maxSize, colorLower, colorUpper, minPresence)

        if self.usingTracker and detected:
            self.clear()
            return self._tracker.init(frame, boundingBox), boundingBox
            
        return detected, boundingBox
    
    def update(self, frame):
        if self.usingTracker:
            delta = datetime.datetime.now() - self.lastDetectRunTime
            if delta.seconds > 1:
                self.init(frame, *self.lastDetectionParameters)
            return self._tracker.update(frame)
        else:
            return self._detect(frame, *self.lastDetectionParameters)
        
    def clear(self):
        if(self._tracker is not None):
            self._tracker.clear()
        self._selectTracker()

    def _detect(self, frame, minSize, maxSize, colorLower, colorUpper, minPresence):
        self.lastDetectRunTime = datetime.datetime.now()
        self.lastDetectionParameters = (minSize, maxSize, colorLower, colorUpper, minPresence)
        
        userColor = (colorLower is not None) and (colorUpper is not None)
        minPresence *= 255.0
        
        objects = self._cascadeDetector.detectMultiScale(frame, scaleFactor=1.3, minNeighbors=5, flags=0, minSize=minSize, maxSize=maxSize)

        for obj in objects:
            boundingBox = tuple(map(lambda a: int(a), obj))
            (x, y, w, h) = boundingBox
            if(userColor):
                objInHVS = cv2.cvtColor(frame[y:y+h, x:x+w], cv2.COLOR_BGR2HSV)
                mask = cv2.inRange(objInHVS, colorLower, colorUpper)
                presence = cv2.mean(mask)[0]
                if(presence < minPresence):
                    # Se não tiver essa porcentagem de cor, pula esse objeto
                    continue

            return True, boundingBox
            
        return False, None
    
    def _selectTracker(self):
        if self.methodName == '':
            self._tracker = None
        elif self.methodName == 'BOOSTING':
            self._tracker = cv2.TrackerBoosting_create()
        elif self.methodName == 'MIL':
            self._tracker = cv2.TrackerMIL_create()
        elif self.methodName == 'KCF':
            self._tracker = cv2.TrackerKCF_create()
        elif self.methodName == 'TLD':
            self._tracker = cv2.TrackerTLD_create()
        elif self.methodName == 'MEDIANFLOW':
            self._tracker = cv2.TrackerMedianFlow_create()
        elif self.methodName == 'GOTURN':
            self._tracker = cv2.TrackerGOTURN_create()
        else:
            print("Algortimo '" + self.methodName + "' não reconhecido")
            self.methodName = ""
            
        self.usingTracker = not(self._tracker is None)
