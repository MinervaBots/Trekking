import cv2

class Detection:
    def __init__(self, cascadePath, drawInTheFrame, rectColor, scaleFactor, minNeighbors, flags):
        self._cascadeDetector = cv2.CascadeClassifier(cascadePath)
        self.drawInTheFrame = drawInTheFrame
        self.rectColor = rectColor
        self.scaleFactor = scaleFactor
        self.minNeighbors = minNeighbors
        self.flags = flags
        #self.minSize = minSize
        #self.maxSize = maxSize
    
    def detect(self, frame, colorLower = None, colorUpper = None, minPresence = 0.0):
        userColor = (colorLower is not None) and (colorUpper is not None)
        minPresence *= 255.0
        
        # Haar Cascade trabalha em escala de cinza
        # Então converte a imagem aqui
        # Futuramente vou implementar um controle de cor
        frameInGray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        objects = list(self._cascadeDetector.detectMultiScale(frameInGray, self.scaleFactor, self.minNeighbors, self.flags))

        if(self.drawInTheFrame and len(objects) > 0):
            for i in range(len(objects) - 1, -1, -1):
                (x, y, w, h) = objects[i]
                if(userColor):
                    objInHVS = cv2.cvtColor(frame[y:y+h, x:x+w], cv2.COLOR_BGR2HSV)
                    mask = cv2.inRange(objInHVS, colorLower, colorUpper)
                    #cv2.imshow("mask", mask)
                    presence = cv2.mean(mask)[0]
                    if(presence < minPresence):
                        del objects[i]
                        continue
                    
                cv2.rectangle(frame, (x, y), (x + w, y + h), self.rectColor, 2)
        return objects
