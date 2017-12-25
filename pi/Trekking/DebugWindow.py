import cv2

class DebugWindow:
    def __init__(self, enabled, windowName, width, height):
        self.enabled = enabled
        self.width = width
        self.height = height
        self.windowName = windowName

    def open(self):
        if self.enabled:
            cv2.namedWindow(self.windowName)
            cv2.resizeWindow(self.windowName, self.width, self.height)
            
    def update(self, frame):
        if self.enabled:
            cv2.imshow(self.windowName, frame)
        cv2.waitKey(1)
        
    def close(self):
        if self.enabled:
            cv2.destroyWindow(self.windowName)
