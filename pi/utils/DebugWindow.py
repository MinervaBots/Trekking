import cv2

class DebugWindow:
    def __init__(self, enabled, windowName, resolution):
        self.enabled = enabled
        self.width = resolution[0]
        self.height = resolution[1]
        self.windowName = windowName
        
    def open(self):
        if self.enabled:
            cv2.namedWindow(self.windowName)
            cv2.resizeWindow(self.windowName, self.width, self.height)
            
    def update(self, frame):
        if self.enabled:
            cv2.imshow(self.windowName, frame)
            return cv2.waitKey(1)
        return -1
        
    def close(self):
        if self.enabled:
            cv2.destroyWindow(self.windowName)

    def rectangle(self, frame, p1, p2, color, thickness = 1, lineType = cv2.LINE_8, shift = 0):
        if(self.enabled):
            cv2.rectangle(frame, p1, p2, color, thickness, lineType, shift)

    def putTextWarning(self, frame, text, org, fontFace, fontScale, thickness = 1, lineType = cv2.LINE_8, bottomLeftOrigin = False):
        if(self.enabled):
            cv2.putText(frame, text, org, fontFace, fontScale, (0, 255, 255), thickness, lineType, bottomLeftOrigin)
        else:
            print("[WARNING]: " + text)

    def putTextInfo(self, frame, text, org, fontFace, fontScale, thickness = 1, lineType = cv2.LINE_8, bottomLeftOrigin = False):
        if(self.enabled):
            cv2.putText(frame, text, org, fontFace, fontScale, (0, 255, 0), thickness, lineType, bottomLeftOrigin)
        else:
            print("[INFO]: " + text)

    def putTextError(self, frame, text, org, fontFace, fontScale, thickness = 1, lineType = cv2.LINE_8, bottomLeftOrigin = False):
        if(self.enabled):
            cv2.putText(frame, text, org, fontFace, fontScale, (0, 0, 255), thickness, lineType, bottomLeftOrigin)
        else:
            print("[ERROR]: " + text)

    def putTextDebug(self, frame, text, org, fontFace, fontScale, thickness = 1, lineType = cv2.LINE_8, bottomLeftOrigin = False):
        if(self.enabled):
            cv2.putText(frame, text, org, fontFace, fontScale, (255, 0, 0), thickness, lineType, bottomLeftOrigin)
        else:
            print("[DEBUG]: " + text)

    def putText(self, frame, text, org, fontFace, fontScale, color, thickness = 1, lineType = cv2.LINE_8, bottomLeftOrigin = False):
        if(self.enabled):
            cv2.putText(frame, text, org, fontFace, fontScale, color, thickness, lineType, bottomLeftOrigin)
        else:
            print(text)
