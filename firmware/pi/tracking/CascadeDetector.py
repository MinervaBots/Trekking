import cv2
from .Detector import Detector
    
class CascadeDetector(Detector):
    def __init__(self, resolution, cascadePath, scaleFactor = 1.05, minNeighbors = 5, flags = 0, minSize = (0,0), maxSize = (0,0), colorIntervals = [], minPresence = 0.0):
        Detector.__init__(self, resolution)
        self.__cascadeDetector = cv2.CascadeClassifier(cascadePath)
        self.scaleFactor = scaleFactor
        self.minNeighbors = minNeighbors
        self.flags = flags
        self.minSize = minSize
        self.maxSize = maxSize
        self.minPresence = minPresence * 255.0
        self.colorIntervals = colorIntervals
    
    def find(self, frame):
        resultObjs = []
        resultDirections = []
        
        userColor = len(self.colorIntervals) > 0
        
        objects = self.__cascadeDetector.detectMultiScale(frame, self.scaleFactor, self.minNeighbors, self.flags, self.minSize, self.maxSize)

        for obj in objects:
            (x, y, w, h) = obj
            
            if(userColor):
                presence = 0
                objInHVS = cv2.cvtColor(frame[y:y+h, x:x+w], cv2.COLOR_BGR2HSV)
                
                for upperColor, lowerColor in self.colorIntervals:
                    mask = cv2.inRange(objInHVS, lowerColor, upperColor)
                    presence += cv2.mean(mask)[0]
                    
                if(presence < self.minPresence):
                    # Se não tiver essa porcentagem de cor, pula esse objeto
                    continue
            
            #print(obj)
            resultObjs.append(obj)
            resultDirections.append(self.rectToDirection(obj))
            
        return len(resultObjs) > 0, resultObjs, resultDirections
    