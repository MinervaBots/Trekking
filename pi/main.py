from utils.FPS import FPS
from videoStream.VideoStream import VideoStream
from Trekking.ArduinoCom import *
from Trekking.BluetoothCom import *
from Trekking.DebugWindow import *
from Trekking.Detection import *

import Trekking.ArduinoCommands as ArduinoCommands
import Trekking.BluetoothHandlers as BluetoothHandlers

import cv2
import os
from sys import platform as _platform


isRunning = False
isRunningDetection = False

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

arduino = ArduinoCom(arduinoPort, 9600, arduinoCommands)
bluetooth = BluetoothCom(bluetoothPort, 9600, 0.1, bluetoothHandlers)

window = DebugWindow(enableWindow, "debug", 640, 368)
detection = Detection("cascades/face.xml", True, (255, 0, 0), 1.3, 5, 0)

#Medida de performance
fps = FPS(True)

#Captura de video
video = VideoStream(usePiCamera = isRaspberryPi, resolution = (window.width, window.height))

def setup():
    bluetooth.start()
    
    while not isRunning:
        continue

    window.open()
    video.start()
    arduino.start()
    fps.start()

    while isRunning:
        try:
            loop()
            fps.update()
        except KeyboardInterrupt:
            stop(None)

    fps.stop()
    arduino.close()
    bluetooth.close()
    window.close()
    video.stop()
    

def loop():
    frame = video.read()
    if frame is None:
        print(u'Não foi possível recuperar um frame da câmera')
        return None

    if(isRunningDetection):
        #objects = detection.detect(frame, (5, 50, 50), (65, 255, 255))
        objects = detection.detect(frame, (0, 0, 0), (360, 255, 255))
        detected = len(objects) > 0
        if detected:
            (x, y, w, h) = objects[0]
            objCenterX = x + (w / 2.0)

            # Faz uma interpolação para calcular a direção
            # do centro do objeto relativo ao centro da tela
            direction = (objCenterX - 0) * (1 - (-1)) / (video.width - 0) + (-1)
            arduino.send("targetData", direction, x, y, w, h)
    window.update(frame)
    
setup()
