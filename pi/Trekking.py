from utils.FPS import FPS
from videoStream.VideoStream import VideoStream
from threading import Thread, Lock
import cv2
import time
import PyCmdMessenger
import CommandMessager

isRunning = True

#Configurações da imagem
showWindows = True
windowName = "cameraFeed"
frameWidth = 640
frameHeight = 368
frameCenterX = frameWidth / 2.0

def relay(msg, recvTime):
    #print (msg)
    return None

def servoDirection(position, recvTime):
    print(position)
    
#PyCmdMessage
lock = Lock()
arduino = PyCmdMessenger.ArduinoBoard("COM3", baud_rate = 9600)
commands = [["targetData", "fffff"],
            ["servoDirection", "f", servoDirection],
            ["relay", "s"]]
cmdMessenger = CommandMessager.CommandMessager(arduino, commands, lock)

#Medida de performance
fps = FPS()

#Captura de video
vs = VideoStream(usePiCamera=False, resolution=(frameWidth, frameHeight)).start()

#cascadeDetector = cv2.CascadeClassifier('/home//pi//Trekking//Code//Pi//Full//Python//haarCascade.xml')
cascadeDetector = cv2.CascadeClassifier('haarCascades/face.xml')

def setup():
    #Só inicializa a janela se solicitarmos
    if showWindows:
        cv2.namedWindow(windowName)
        cv2.resizeWindow(windowName, frameWidth, frameHeight)
        
    cmdMessenger.start()
    fps.start()
    while isRunning:
        loop()

    fps.stop()
    cmdMessenger.stop()
    print("[INFO] elasped time: {:.2f}".format(fps.elapsed()))
    print("[INFO] Mean. FPS: {:.2f}".format(fps.meanFps()))
    cv2.destroyAllWindows()
    vs.stop()
    arduino.close()
        
def loop():
    global isRunning, cnt
    frame = vs.read()
    if frame is None:
        print(u'Não foi possível recuperar um frame da câmera')
        return None
        
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    objects = cascadeDetector.detectMultiScale(gray, 1.3, 5)
    if len(objects) > 0:
        (x, y, w, h) = objects[0]
        objCenterX = x + w / 2.0
        direction = (objCenterX - 0) * (1 - -1) / (frameWidth - 0) + -1
        #print(direction)
        cmdMessenger.send("targetData", direction, x, y, w, h)
        cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)

    if showWindows:
        cv2.imshow(windowName, frame)
        if cv2.waitKey(1) == 27:
            isRunning = False

        fps.update()
        #print("FPS: {:.2f}".format(fps.fps()))

setup()
