import cv2
import datetime
#from scipy.interpolate import interp1d

class Tracker:
    def __init__(self, cascadePath, resolution, trackingMethodName = "CASCADE"):
        self.__tracker = None
        self.usingTracker = False
        self.__cascadeDetector = cv2.CascadeClassifier(cascadePath)
        self.setTrackingMethod(trackingMethodName)
        self.resolution = resolution
        self.isRunning = False
        
    def setTrackingMethod(self, trackingMethodName):
        self.methodName = trackingMethodName
        self.__selectTracker()

            
    def pause(self):
        self.isRunning = False
        
    def start(self):
        self.isRunning = True
        
    def init(self, frame, minSize = (0,0), maxSize = (0,0), colorLower = None, colorUpper = None, minPresence = 0.0):
        if not self.isRunning:
            return False, None, 0
            
        detected, boundingBox, direction = self.__detect(frame, minSize, maxSize, colorLower, colorUpper, minPresence)

        if self.usingTracker and detected:
            self.clear()
            boundingBox = tuple(boundingBox)
            return self.__tracker.init(frame, boundingBox), boundingBox, self.__rectToDirection(boundingBox)
            
        return detected, boundingBox, direction
    
    def update(self, frame):
        if not self.isRunning:
            return False, None, 0
            
        if self.usingTracker:
            delta = datetime.datetime.now() - self.lastDetectRunTime
            if delta.seconds > 3:
                return self.init(frame, *self.lastDetectionParameters)

            found, boundingBox = self.__tracker.update(frame)
            return found, boundingBox, self.__rectToDirection(boundingBox)
        else:
            return self.__detect(frame, *self.lastDetectionParameters)
        
    def clear(self):
        if(self.__tracker is not None):
            self.__tracker.clear()
        self.__selectTracker()

    def __detect(self, frame, minSize, maxSize, colorLower, colorUpper, minPresence):
        self.lastDetectRunTime = datetime.datetime.now()
        self.lastDetectionParameters = (minSize, maxSize, colorLower, colorUpper, minPresence)
        
        userColor = (colorLower is not None) and (colorUpper is not None)
        minPresence *= 255.0
        
        objects = self.__cascadeDetector.detectMultiScale(frame, scaleFactor=1.3, minNeighbors=5, flags=0, minSize=minSize, maxSize=maxSize)

        for obj in objects:
            (x, y, w, h) = obj
            if(userColor):
                objInHVS = cv2.cvtColor(frame[y:y+h, x:x+w], cv2.COLOR_BGR2HSV)
                mask = cv2.inRange(objInHVS, colorLower, colorUpper)
                presence = cv2.mean(mask)[0]
                if(presence < minPresence):
                    # Se não tiver essa porcentagem de cor, pula esse objeto
                    continue
                
            return True, obj, self.__rectToDirection(obj)
            
        return False, None, 0
    
    def __selectTracker(self):
        if self.methodName == '' or self.methodName == 'CASCADE':
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
        else:
            print("Algortimo '" + self.methodName + "' não reconhecido")
            self.methodName = ""
            
        self.usingTracker = not(self.__tracker is None)

    # Faz uma interpolação para calcular a direção
    # do centro do objeto relativo ao centro da tela
    # Mapeia a posição em pixels na tela para uma direção entre -1 e 1
    def __rectToDirection(self, rect):
        objCenterX = rect[0] + (rect[2] / 2.0)
        #direction = float(interp1d([0, self.resolution[0]], [-1, 1])(objCenterX))
        return 0

    @staticmethod
    def rectToPoints(rect):
        p1 = rect[0], rect[1]
        p2 = rect[0] + rect[2], rect[1] + rect[3]
        return p1, p2