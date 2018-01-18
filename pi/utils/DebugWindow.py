import cv2
from colorama import init, Fore

class DebugWindow:
    def __init__(self, enabled, windowName, width, height):
        self.enabled = enabled
        self.width = width
        self.height = height
        self.windowName = windowName
        init(convert=True)
        
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

    def rectangle(self, frame, p1, p2, color, thickness = 1, lineType = cv2.LINE_8, shift = 0):
        if(self.enabled):
            cv2.rectangle(frame, p1, p2, color, thickness, lineType, shift)

    def putTextWarning(self, frame, text, org, fontFace, fontSccale, thickness = 1, lineType = cv2.LINE_8, bottomLeftOrigin = False):
        if(self.enabled):
            cv2.putText(frame, text, org, fontFace, fontSccale, (0, 255, 255), thickness, lineType, bottomLeftOrigin)
        else:
            print(Fore.YELLOW + text)

    def putTextInfo(self, frame, text, org, fontFace, fontSccale, thickness = 1, lineType = cv2.LINE_8, bottomLeftOrigin = False):
        if(self.enabled):
            cv2.putText(frame, text, org, fontFace, fontSccale, (0, 255, 0), thickness, lineType, bottomLeftOrigin)
        else:
            print(Fore.GREEN + text)

    def putTextError(self, frame, text, org, fontFace, fontSccale, thickness = 1, lineType = cv2.LINE_8, bottomLeftOrigin = False):
        if(self.enabled):
            cv2.putText(frame, text, org, fontFace, fontSccale, (0, 0, 255), thickness, lineType, bottomLeftOrigin)
        else:
            print(Fore.RED + text)

    def putTextDebug(self, frame, text, org, fontFace, fontSccale, thickness = 1, lineType = cv2.LINE_8, bottomLeftOrigin = False):
        if(self.enabled):
            cv2.putText(frame, text, org, fontFace, fontSccale, (255, 0, 0), thickness, lineType, bottomLeftOrigin)
        else:
            print(Fore.BLUE + text)

    def putText(self, frame, text, org, fontFace, fontSccale, color, thickness = 1, lineType = cv2.LINE_8, bottomLeftOrigin = False):
        if(self.enabled):
            cv2.putText(frame, text, org, fontFace, fontSccale, color, thickness, lineType, bottomLeftOrigin)
        else:
            print(text)
