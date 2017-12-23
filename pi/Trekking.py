from utils.FPS import FPS
from videoStream.VideoStream import VideoStream
from threading import Thread
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

def relayToPC(msg, recvTime):
    # Defalt handler
    # Caso nenhum handler seja definido usamos esse
    # Futuramente ele vai simplesmente encaminhar a mensagem para o PC
    return

def error(msg, recvTime):
    print ("ERROR: {}".format(msg))

def servoDirection(position, recvTime):
    print(position)

#PyCmdMessage
arduino = PyCmdMessenger.ArduinoBoard("COM3", baud_rate = 9600)
commands = [["targetData1", "fffff"],
            ["servoDirection", "f", servoDirection],
            ["error", "s", error]]
cmdMessenger = CommandMessager.CommandMessager(arduino, commands, relayToPC)

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
    if len(objects) > 0:
        (x, y, w, h) = objects[0]
        objCenterX = x + w / 2.0

        # Faz uma interpolação para calcular a direção
        # do centro do objeto relativo ao centro da tela
        direction = (objCenterX - 0) * (1 - (-1)) / (frameWidth - 0) + (-1)
        cmdMessenger.send("targetData1", direction, x, y, w, h)
        cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)

    if showWindows:
        cv2.imshow(windowName, frame)
        if cv2.waitKey(1) == 27:
            isRunning = False

        fps.update()
        #print("FPS: {:.2f}".format(fps.fps()))

setup()
