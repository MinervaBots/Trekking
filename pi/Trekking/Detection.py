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
        
    def detect(self, frame):
        # Haar Cascade trabalha em escala de cinza
        # Então converte a imagem aqui
        # Futuramente vou implementar um controle de cor
        frameInGray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        objects = self._cascadeDetector.detectMultiScale(frameInGray, self.scaleFactor, self.minNeighbors, self.flags)

        if(self.drawInTheFrame and len(objects) > 0):
            for (x, y, w, h) in objects:
                cv2.rectangle(frame, (x, y), (x + w, y + h), self.rectColor, 2)

        return objects
