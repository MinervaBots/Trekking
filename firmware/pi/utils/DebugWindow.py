import cv2

class DebugWindow:
    def __init__(self, enabled : bool, windowName : str, resolution, delay : int, record : bool):
        self.enabled = enabled
        self.width = int(resolution[0])
        self.height = int(resolution[1])
        self.windowName = windowName
        self.delay = delay
        self.record = record
        
        if self.record:
            self.out = cv2.VideoWriter('output.mp4', cv2.VideoWriter_fourcc('F','M','P', '4'), 60, (self.width, self.height))

    def open(self):
        if self.enabled:
            cv2.namedWindow(self.windowName)
            cv2.resizeWindow(self.windowName, self.width, self.height)
            
    def update(self, frame):
        if self.record:
            self.out.write(frame)

        if self.enabled:
            cv2.imshow(self.windowName, frame)
            return cv2.waitKey(self.delay)
        return -1
        
    def close(self):
        if self.record:
            self.out.release()
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

    def drawTargets(self, frame, targets, thickness = 1, lineType = cv2.LINE_8):
        for i, target in enumerate(targets):
            p1, p2 = DebugWindow.rectToPoints(target.rect)
            org = ( int((p1[0] + p2[0]) / 2), int((p1[1] + p2[1]) / 2))
            self.rectangle(frame, p1, p2, target.color, 2, 1)
            self.putText(frame, str(i), org, cv2.FONT_HERSHEY_SIMPLEX, 0.75, target.color, 2)
            
    @staticmethod
    def rectToPoints(rect):
        p1 = int(rect[0]), int(rect[1])
        p2 = int(rect[0] + rect[2]), int(rect[1] + rect[3])
        return p1, p2