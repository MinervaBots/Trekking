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
from targeting.Target import Target
import time

from messaging.ArduinoMessagingThread import ArduinoMessagingThread
from messaging.BluetoothMessagingThread import BluetoothMessagingThread
from messaging.CommonMessageHandlers import *
from messaging.ArduinoMessageHandlers import *
from messaging.ArduinoCommands import *
from messaging.BluetoothMessageHandlers import *

directionFilter = SimpleLowPassFilter(0.5)
distanceFilter = SimpleLowPassFilter(0.5)

systemInfo = SystemInfo()

#Captura de video
video = VideoStream(usePiCamera = systemInfo.isRaspberryPi, framerate = 30, resolution = (640, 368))
video.start() # Inicializa a câmera aqui pra ter tempo de esquentar se for no Raspberry Pi

detector = CascadeDetector(video.resolution, "cascades/face.xml")
tracker = OpenCVTracker(detector, 500, "MEDIANFLOW")
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
arduinoMessagingThread = container.resolve(ArduinoMessagingThread)
bluetoothMessagingThread = container.resolve(BluetoothMessagingThread)

lastUpdateTime = 0
targets = []
            
def setup():
    video.setCameraFocalLenght(459) # Valor calculado para a câmera do notebook do Paulo
    arduinoMessagingThread.setPort(systemInfo.arduinoPort)
    bluetoothMessagingThread.setPort(systemInfo.bluetoothPort)

    arduinoMessagingThread.start()
    bluetoothMessagingThread.start()
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
        (systemInfo.isTracking, systemInfo.trackedRects, systemInfo.trackedDirections) = tracker.init(frame)
        window.putTextWarning(frame, "Tentando detectar...", (20, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.75, 2)
        arduinoMessagingThread.send(MessageCodes.TARGET_LOST)
    else:   
        (systemInfo.isTracking, systemInfo.trackedRects, systemInfo.trackedDirections) = tracker.update(frame)

        if len(systemInfo.trackedRects) == 0:
            arduinoMessagingThread.send(MessageCodes.TARGET_LOST)
            window.putTextError(frame, "Falha detectada no rastreamento", (20, 80), cv2.FONT_HERSHEY_SIMPLEX, 0.75, 2)
        else:
            for i, rect in enumerate(systemInfo.trackedRects):
                distance = video.calculateDistance(rect[2], 0.2)
                direction = systemInfo.trackedDirections[i]
                if len(targets) <= i:
                    targets.append(Target(rect, distance, direction))
                else:
                    targets[i].update(rect, distance, direction)
            
            sortedTargets = sorted(targets[:len(systemInfo.trackedRects)], key = lambda t : t.distance)
            window.drawTargets(frame, sortedTargets, 2, 1)
            currentTarget = sortedTargets[0]
            
            filteredDirection = directionFilter.calculate(currentTarget.direction)
            filteredDistance = distanceFilter.calculate(currentTarget.distance)
            
            arduinoMessagingThread.send(MessageCodes.TARGET_FOUND, filteredDirection, filteredDistance)
            window.putTextInfo(frame, tracker.methodName + ": ({:.2f}, {:.2f})".format(filteredDistance, filteredDirection), (20, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.75, 2)

    tempValue = temp.update()
    window.putTextInfo(frame, "({:.0f}, {:.2f})".format(*fps.update()) + " - Temp: " + str(tempValue) + " 'C", (20, 50), cv2.FONT_HERSHEY_SIMPLEX, 0.75, 2)

    if(time.time() - lastUpdateTime > 2):
        arduinoMessagingThread.send(MessageCodes.TEMPERATURE, tempValue)
        lastUpdateTime = time.time()
        
    # ESC pressionado
    return window.update(frame) == 27

def stop():
    arduinoMessagingThread.clearSendQueue()
    arduinoMessagingThread.send(MessageCodes.STOP_EVENT)
    cv2.waitKey(500)
    
    fps.stop(True)
    arduinoMessagingThread.close()
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
