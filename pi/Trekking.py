from utils.FPS import FPS
from videoStream.VideoStream import VideoStream
from threading import Thread
import cv2
import PyCmdMessenger
import CommandMessenger
import os
from sys import platform as _platform

isRunning = True
isRaspberryPi = "linux" in _platform

# Configurações da imagem
# Habilita as janelas caso estejamos em desktop mode ou no Windows
showWindows = "DISPLAY" in os.environ if isRaspberryPi else True

windowName = "cameraFeed"
frameWidth = 640
frameHeight = 368
frameCenterX = frameWidth / 2.0

def info(msg, recvTime):
    print ("INFO: {}".format(msg))

def error(msg, recvTime):
    print ("ERROR: {}".format(msg))

def mpuData(accX, accY, heading, recvTime):
    print("{} {} {}".format(accX, accY, heading))

#PyCmdMessage
baudRate = 9600
port = "/dev/ttyACM0" if isRaspberryPi else "COM3"
arduino = PyCmdMessenger.ArduinoBoard(port, baud_rate = baudRate)
commands = [["info", "s", info],
            ["error", "s", error],
            ["mpuData", "fff", mpuData],
            ["targetData", "fffff"]]

cmdMessenger = CommandMessenger.CommandMessenger(arduino, commands)

#Medida de performance
fps = FPS()

#Captura de video
vs = VideoStream(usePiCamera = isRaspberryPi, resolution=(frameWidth, frameHeight)).start()

#cascadeDetector = cv2.CascadeClassifier('/home//pi//Trekking//Code//Pi//Full//Python//haarCascade.xml')
cascadeDetector = cv2.CascadeClassifier('haarCascades/face.xml')

def setup():
    global isRunning
    #Só inicializa a janela se solicitarmos
    if showWindows:
        cv2.namedWindow(windowName)
        cv2.resizeWindow(windowName, frameWidth, frameHeight)
        
    cmdMessenger.start()
    fps.start()
    while isRunning:
        try:
            loop()
        except KeyboardInterrupt:
            isRunning = False

    fps.stop()
    cmdMessenger.stop()
    arduino.close()
    cv2.destroyAllWindows()
    vs.stop()
    
    print("[INFO] Tempo percorrido: {:.2f}".format(fps.elapsed()))
    print("[INFO] FPS médio: {:.2f}".format(fps.meanFps()))
        
def loop():
    global isRunning
    frame = vs.read()
    if frame is None:
        print(u'Não foi possível recuperar um frame da câmera')
        return None

    # Haar Cascade trabalha em escala de cinza
    # Então converte a imagem aqui
    # Futuramente vou implementar um controle de cor
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    objects = cascadeDetector.detectMultiScale(gray, 1.3, 5)
    detected = len(objects) > 0
    if detected:
        (x, y, w, h) = objects[0]
        objCenterX = x + w / 2.0

        # Faz uma interpolação para calcular a direção
        # do centro do objeto relativo ao centro da tela
        direction = (objCenterX - 0) * (1 - (-1)) / (frameWidth - 0) + (-1)
        cmdMessenger.send("targetData", direction, x, y, w, h)

    if showWindows:
        if detected:
            cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)
        cv2.imshow(windowName, frame)
        if cv2.waitKey(1) == 27:
            isRunning = False

    fps.update()
    print("FPS: {:.2f}".format(fps.fps()))

setup()
