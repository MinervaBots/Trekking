import cv2
import os
import time
from sys import platform as _platform
from scipy.interpolate import interp1d

from utils.FPS import FPS
from utils.DebugWindow import *
from videoStream.VideoStream import VideoStream
from communication.ArduinoCom import *
from communication.BluetoothCom import *
from tracking.Tracker import *
import communication.ArduinoCommands as ArduinoCommands
import communication.BluetoothHandlers as BluetoothHandlers


isRunning = True
isRunningDetection = True

def start(message):
    global isRunning
    isRunning = True

def stop(message):
    global isRunning
    isRunning = False

def startDetection(message = None):
    global isRunningDetection
    isRunningDetection = True

def pauseDetection(message = None):
    global isRunningDetection
    isRunningDetection = False

    
isRaspberryPi = "linux" in _platform
if(isRaspberryPi):
    arduinoPort = "/dev/ttyACM0"
    bluetoothPort = "/dev/rfcomm0"
    enableWindow = "DISPLAY" in os.environ
else:
    arduinoPort = "COM3"
    bluetoothPort = "COM4"
    enableWindow = True

arduinoCommands = [["info", "s", ArduinoCommands.info],
            ["error", "s", ArduinoCommands.error],
            ["mpuData", "fff", ArduinoCommands.mpuData],
            ["targetData", "fffff"],
            ["startDetection", "", startDetection],
            ["pauseDetection", "", pauseDetection]]

bluetoothHandlers = {"start" : start, "stop" : stop, "startDetection" : startDetection, "pauseDetection" : pauseDetection}

#arduino = ArduinoCom(arduinoPort, 9600, arduinoCommands)
#bluetooth = BluetoothCom(bluetoothPort, 9600, 0.1, bluetoothHandlers)

window = DebugWindow(enableWindow, "debug", 640, 368)
tracker = Tracker("cascades/face.xml", "MEDIANFLOW")

#Medida de performance
fps = FPS(False)

#Captura de video
video = VideoStream(usePiCamera = isRaspberryPi, resolution = (window.width, window.height))

def setup():
    #bluetooth.start()

    if(isRaspberryPi):
        time.sleep(2.0)

    while not isRunning:
        continue

    fps.start()
    window.open()
    video.start()
    #arduino.start()

    while isRunning:
        try:
            fps.update()
            loop()
        except KeyboardInterrupt:
            stop(None)

    fps.stop()
    arduino.close()
    bluetooth.close()
    window.close()
    video.stop()

isTracking = False
boundingBox = (0,0,0,0)

def loop():
    global isTracking, boundingBox
    
    frame = video.read()
    if frame is None:
        print(u'Não foi possível recuperar um frame da câmera')
        return None

    if(isRunningDetection):
        
        if not isTracking:
            (isTracking, boundingBox) = tracker.init(frame)
            cv2.putText(frame, "Trying detect...", (20, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (0, 255, 255), 2)
        else:   
            (isTracking, boundingBox) = tracker.update(frame)

            if boundingBox is not None:
                boundingBox = tuple(map(lambda a: int(a), boundingBox))
                p1 = boundingBox[0], boundingBox[1]
                p2 = boundingBox[0] + boundingBox[2], boundingBox[1] + boundingBox[3]
                window.rectangle(frame, p1, p2, (255,0,0), 2, 1)

                objCenterX = boundingBox[0] + (boundingBox[2] / 2.0)
                # Faz uma interpolação para calcular a direção
                # do centro do objeto relativo ao centro da tela

                # Mapeia a posição em pixels na tela para uma direção entre -1 e 1
                direction = float(interp1d([0,video.width],[-1,1])(objCenterX))
                print(direction)
                #arduino.send("targetData", direction, *boundingBox)
            else:
                window.putTextError(frame, "Tracking failure detected", (20,80), cv2.FONT_HERSHEY_SIMPLEX, 0.75, 2)

            methodName = tracker.methodName
            if methodName == "":
                methodName = "Cascade"
            window.putTextInfo(frame, methodName + " Tracker: " + str(boundingBox), (20,20), cv2.FONT_HERSHEY_SIMPLEX, 0.75, 2)

    window.putTextInfo(frame, "FPS : " + str(int(fps.fps())), (20,50), cv2.FONT_HERSHEY_SIMPLEX, 0.75, 2)
    window.update(frame)
   
setup()
