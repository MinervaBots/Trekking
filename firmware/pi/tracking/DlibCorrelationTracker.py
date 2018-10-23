import cv2
import dlib
from .Tracker import Tracker
 
class DlibCorrelationTracker(Tracker):
    def __init__(self, detector, detectionIntervalMs):
        Tracker.__init__(self, detector, "Correlation Tracker", detectionIntervalMs)
        self.__tracker = dlib.correlation_tracker()
        
    def init(self, frame):
        if not self.isRunning:
            return False, None, 0
            
        detected, boundingBox, direction = self._detect(frame)
        if detected:
            self.clear()
            rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            rect = dlib.rectangle(boundingBox[0], boundingBox[1], boundingBox[0] + boundingBox[2], boundingBox[1] + boundingBox[3])
            self.__tracker.start_track(rgb, rect)
            
        return detected, boundingBox, direction
    
    def clear(self):
        self.__tracker = dlib.correlation_tracker()
    
    def _track(self, frame):
        rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        self.__tracker.update(rgb)
        pos = self.__tracker.get_position()
        startX = int(pos.left())
        startY = int(pos.top())
        endX = int(pos.right())
        endY = int(pos.bottom())
        boundingBox = [startX, startY, endX - startX, endY - startY]
        return not pos.is_empty(), boundingBox, self.detector.rectToDirection(boundingBox)