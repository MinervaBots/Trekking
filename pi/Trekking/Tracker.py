import cv2

class Tracker:
    def __init__(self, trackingMethodName):
        self.methodName = trackingMethodName
        self._selectTracker()
        
    def init(self, frame, boundingBox):
        return self._tracker.init(frame, boundingBox)

    def update(self, frame):
        return self._tracker.update(frame)
        
    def clear(self):
        self._tracker.clear()
        self._selectTracker()

    def _selectTracker(self):
        if self.methodName == 'BOOSTING':
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
