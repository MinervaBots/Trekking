#!/usr/bin/env python
import cv2
import dic

from SystemInfo import SystemInfo
from utils.PerfCounter import PerfCounter
from utils.DebugWindow import DebugWindow
from utils.TemperatureControl import TemperatureControl
from utils.Filters import *
from videoStream.VideoStream import VideoStream
from tracking.Detector import Detector
from tracking.CascadeDetector import CascadeDetector
from tracking.Tracker import Tracker
from tracking.OpenCVTracker import OpenCVTracker
import time

from messaging.ArduinoMessagingThread import ArduinoMessagingThread
from messaging.BluetoothMessagingThread import BluetoothMessagingThread
from messaging.CommonMessageHandlers import *
from messaging.ArduinoMessageHandlers import *
from messaging.ArduinoCommands import *
from messaging.BluetoothMessageHandlers import *

directionFilter = RunningAverageFilter()
distanceFilter = RunningAverageFilter(100)

systemInfo = SystemInfo()

#Captura de video
video = VideoStream(usePiCamera = systemInfo.isRaspberryPi, framerate = 30, resolution = (640, 368))
video.start() # Inicializa a câmera aqui pra ter tempo de esquentar se for no Raspberry Pi

detector = CascadeDetector(video.resolution, "cascades/face.xml")
tracker = OpenCVTracker(detector, 2000, "MEDIANFLOW")
window = DebugWindow(systemInfo.enableWindow, "debug", video.resolution, 1, False)

#Medida de performance
fps = PerfCounter(False)
temp = TemperatureControl(1)

builder = dic.container.ContainerBuilder()

# Registra as threads de comunicação. Os Handlers vão ser resolvidos automaticamente
builder.register_class(ArduinoMessagingThread, component_scope=dic.scope.SingleInstance)
builder.register_class(ArduinoCommands, component_scope=dic.scope.SingleInstance)
builder.register_class(BluetoothMessagingThread, component_scope=dic.scope.SingleInstance)
# Registra os handlers
builder.register_module(ArduinoMessageHandlersModule())
builder.register_module(BluetoothMessageHandlersModule())
builder.register_class(StartTrackerHandler, register_as = [ArduinoMessageHandler, BluetoothMessageHandler], component_scope=dic.scope.SingleInstance)
builder.register_class(PauseTrackerHandler, register_as = [ArduinoMessageHandler, BluetoothMessageHandler], component_scope=dic.scope.SingleInstance)


#Registra instancias para serem acessiveis dentro dos handlers
builder.register_instance(SystemInfo, systemInfo)
builder.register_instance(DebugWindow, window)
builder.register_instance(Detector, detector)
builder.register_instance(Tracker, tracker)
builder.register_instance(VideoStream, video)
builder.register_instance(FPS, fps)
builder.register_instance(TemperatureControl, temp)

# Constroi o container
container = builder.build()

# Extrai as instancias com as dependencias já resolvidas
#arduinoMessagingThread = container.resolve(ArduinoMessagingThread)
bluetoothMessagingThread = container.resolve(BluetoothMessagingThread)

lastUpdateTime = 0

def setup():
    video.setCameraFocalLenght(3.04) # Padrão do raspberry pi
    #arduinoMessagingThread.setPort(systemInfo.arduinoPort)
    #bluetoothMessagingThread.setPort(systemInfo.bluetoothPort)

    #arduinoMessagingThread.start()
    #bluetoothMessagingThread.start()
    systemInfo.isRunning = True
    tracker.isRunning = True
    while not systemInfo.isRunning:
        continue
    
    fps.start()
    window.open()
    

def loop():
    global lastUpdateTime
    
    frame = video.read()
    if frame is None:
        return
    
    if not tracker.isRunning:
        pass
    elif not systemInfo.isTracking:
        (systemInfo.isTracking, systemInfo.trackedRect, systemInfo.trackedDirection) = tracker.init(frame)
        window.putTextWarning(frame, "Tentando detectar...", (20, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.75, 2)
        #arduinoMessagingThread.send(MessageCodes.TARGET_LOST)
    else:   
        (systemInfo.isTracking, systemInfo.trackedRect, systemInfo.trackedDirection) = tracker.update(frame)

        if systemInfo.trackedRect is None or systemInfo.trackedDirection == -1:
            #arduinoMessagingThread.send(MessageCodes.TARGET_LOST)
            window.putTextError(frame, "Falha detectada no rastreamento", (20, 80), cv2.FONT_HERSHEY_SIMPLEX, 0.75, 2)
        else:    
            systemInfo.trackedRect = [int(i) for i in systemInfo.trackedRect]
            p1, p2 = Detector.rectToPoints(systemInfo.trackedRect)
            window.rectangle(frame, p1, p2, (255, 255, 0), 2, 1)

            distance = video.calculateDistance(systemInfo.trackedRect[2], 0.5)
            
            filteredDirection = directionFilter.calculate(systemInfo.trackedDirection)
            filteredDistance = distanceFilter.calculate(distance)
            # Mapeia a posição em pixels na tela para uma direção entre -1 e 1
            #arduinoMessagingThread.send(MessageCodes.TARGET_FOUND, systemInfo.trackedDirection, filteredDistance)
            window.putTextInfo(frame, tracker.methodName + ": " + str(filteredDistance), (20, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.75, 2)

    window.putTextInfo(frame, "({:.0f}, {:.2f})".format(*fps.update()) + " - Temp: " + str(temp.update()) + " 'C", (20, 50), cv2.FONT_HERSHEY_SIMPLEX, 0.75, 2)

    if(lastUpdateTime - time.time() > 2):
        arduinoMessagingThread.send(MessageCodes.TEMPERATURE, temp.update())
        lastUpdateTime = time.time() 
    # ESC pressionado
    return window.update(frame) == 27

def stop():
    #arduinoMessagingThread.clearSendQueue()
    #arduinoMessagingThread.send(MessageCodes.STOP_EVENT)
    cv2.waitKey(500);
    
    fps.stop(True)
    #arduinoMessagingThread.close()
    bluetoothMessagingThread.close()
    window.close()
    video.stop()
    systemInfo.isRunning = False
    
def main():
    setup()
    
    while systemInfo.isRunning:
        try:
            if(loop() == True):
                break
            
        except KeyboardInterrupt:
            break
    stop()
main()
