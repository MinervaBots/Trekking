#!/usr/bin/env python
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
isRunningTracker = True

def start(message):
    global isRunning
    isRunning = True

def stop(message):
    global isRunning
    isRunning = False

def startTracker(message = None):
    global isRunningTracker
    isRunningTracker = True

def pauseTracker(message = None):
    global isRunningTracker
    isRunningTracker = False

def setTrackerMethod(message):
    methodName = str(message[0])
    tracker.setTrackerMethod(methodName)
    
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
            ["startTracker", "", startTracker],
            ["pauseTracker", "", pauseTracker]]

bluetoothHandlers = {"start" : start, "stop" : stop, "startTracker" : startTracker, "pauseTracker" : pauseTracker, "setTrackerMethod" : setTrackerMethod}

#arduino = ArduinoCom(arduinoPort, 9600, arduinoCommands)
bluetooth = BluetoothCom(bluetoothPort, 9600, 0.1, bluetoothHandlers)

window = DebugWindow(enableWindow, "debug", 640, 368)
tracker = Tracker("cascades/face.xml", "MEDIANFLOW")

#Captura de video
video = VideoStream(usePiCamera = isRaspberryPi, framerate=60, resolution = (window.width, window.height))

#Medida de performance
fps = FPS(False)

def setup():
    bluetooth.start()

    while not isRunning:
        continue

    window.open()
    video.start()
    
    if(isRaspberryPi):
        time.sleep(2.0)

    #arduino.start()

    fps.start()
    while isRunning:
        try:
            loop()
        except KeyboardInterrupt:
            stop(None)

    fps.stop(True)
    
    #arduino.close()
    bluetooth.close()
    window.close()
    video.stop()

isTracking = False

def loop():
    global isTracking, isRunning
    
    frame = video.read()
    if frame is None:
        print(u'Não foi possível recuperar um frame da câmera')
        return None

    if(isRunningTracker):
        
        if not isTracking:
            (isTracking, boundingBox) = tracker.init(frame)
            window.putTextWarning(frame, "Trying detect...", (20, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.75, 2)
        else:   
            (isTracking, boundingBox) = tracker.update(frame)

            if boundingBox is not None:
                boundingBox = tuple(map(int, boundingBox))
                p1 = boundingBox[0], boundingBox[1]
                p2 = boundingBox[0] + boundingBox[2], boundingBox[1] + boundingBox[3]
                window.rectangle(frame, p1, p2, (255,0,0), 2, 1)

                objCenterX = boundingBox[0] + (boundingBox[2] / 2.0)
                # Faz uma interpolação para calcular a direção
                # do centro do objeto relativo ao centro da tela

                # Mapeia a posição em pixels na tela para uma direção entre -1 e 1
                direction = float(interp1d([0,video.width],[-1,1])(objCenterX))
                #arduino.send("targetData", direction, *boundingBox)
            else:
                window.putTextError(frame, "Tracking failure detected", (20,80), cv2.FONT_HERSHEY_SIMPLEX, 0.75, 2)

            window.putTextInfo(frame, tracker.methodName + " Tracker: " + str(boundingBox), (20,20), cv2.FONT_HERSHEY_SIMPLEX, 0.75, 2)

    window.putTextInfo(frame, "FPS : " + str(int(fps.update())), (20,50), cv2.FONT_HERSHEY_SIMPLEX, 0.75, 2)

    # ESC pressionado
    if window.update(frame) == 27:
        isRunning = False

    
   
setup()
