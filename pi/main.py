#!/usr/bin/env python
import cv2
import dic

from SystemInfo import SystemInfo
from utils.FPS import FPS
from utils.DebugWindow import DebugWindow
from utils.TemperatureControl import TemperatureControl
from videoStream.VideoStream import VideoStream
from tracking.Tracker import Tracker

from messaging.ArduinoMessagingThread import ArduinoMessagingThread
from messaging.BluetoothMessagingThread import BluetoothMessagingThread
from messaging.CommonMessageHandlers import *
from messaging.ArduinoMessageHandlers import *
from messaging.BluetoothMessageHandlers import *

systemInfo = SystemInfo()
tracker = Tracker("cascades/face.xml", (640, 368), "MEDIANFLOW")
window = DebugWindow(systemInfo.enableWindow, "debug", tracker.resolution)

#Captura de video
video = VideoStream(usePiCamera = systemInfo.isRaspberryPi, framerate = 30, resolution = tracker.resolution)
video.start() # Inicializa a câmera aqui pra ter tempo de esquentar se for no Raspberry Pi

#Medida de performance
fps = FPS(False)
temp = TemperatureControl(1)

builder = dic.container.ContainerBuilder()

# Registra as threads de comunicação. Os Handlers vão ser resolvidos automaticamente
builder.register_class(ArduinoMessagingThread)
builder.register_class(BluetoothMessagingThread)
# Registra os handlers
builder.register_module(BluetoothMessageHandlersModule())
builder.register_module(ArduinoMessageHandlersModule())
builder.register_class(StartTrackerHandler, register_as = [ArduinoMessageHandler, BluetoothMessageHandler], component_scope=dic.scope.SingleInstance)
builder.register_class(PauseTrackerHandler, register_as = [ArduinoMessageHandler, BluetoothMessageHandler], component_scope=dic.scope.SingleInstance)


#Registra instancias para serem acessiveis dentro dos handlers
builder.register_instance(SystemInfo, systemInfo)
builder.register_instance(DebugWindow, window)
builder.register_instance(Tracker, tracker)
builder.register_instance(VideoStream, video)
builder.register_instance(FPS, fps)
builder.register_instance(TemperatureControl, temp)

# Constroi o container
container = builder.build()

# E extrai as instancias com as dependencias já resolvidas
arduinoMessagingThread = container.resolve(ArduinoMessagingThread)
bluetoothMessagingThread = container.resolve(BluetoothMessagingThread)


def setup():
    arduinoMessagingThread.setPort(systemInfo.arduinoPort)
    bluetoothMessagingThread.setPort(systemInfo.bluetoothPort)

    #arduinoMessagingThread.start()
    bluetoothMessagingThread.start()
    
    while not systemInfo.isRunning:
        continue
    
    fps.start()
    window.open()
    while systemInfo.isRunning:
        try:
            loop()
        except KeyboardInterrupt:
            stop(None)

    fps.stop(True)
    
    #arduinoMessagingThread.close()
    bluetoothMessagingThread.close()
    window.close()
    video.stop()

def loop():
    frame = video.read()
    if frame is None:
        return

    if not tracker.isRunning:
        pass
    elif not systemInfo.isTracking:
        (systemInfo.isTracking, systemInfo.trackedRect, systemInfo.trackedDirection) = tracker.init(frame)
        window.putTextWarning(frame, "Tentando detectar...", (20, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.75, 2)
    else:   
        (systemInfo.isTracking, systemInfo.trackedRect, systemInfo.trackedDirection) = tracker.update(frame)

        if systemInfo.trackedRect is None:
            window.putTextError(frame, "Falha detectada no rastreamento", (20, 80), cv2.FONT_HERSHEY_SIMPLEX, 0.75, 2)
        else:    
            systemInfo.trackedRect = [int(i) for i in systemInfo.trackedRect]
            p1, p2 = Tracker.rectToPoints(systemInfo.trackedRect)
            window.rectangle(frame, p1, p2, (255, 0, 0), 2, 1)
            
            # Mapeia a posição em pixels na tela para uma direção entre -1 e 1
            #arduinoMessagingThread.send(ArduinoMessageCodes.TARGET_DATA, direction, *systemInfo.trackedRect)
            window.putTextInfo(frame, tracker.methodName + ": " + str(systemInfo.trackedRect), (20, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.75, 2)

    window.putTextInfo(frame, "FPS : " + str(int(fps.update())) + " - Temp: " + str(temp.update()) + " 'C", (20, 50), cv2.FONT_HERSHEY_SIMPLEX, 0.75, 2)

    # ESC pressionado
    if window.update(frame) == 27:
        systemInfo.isRunning = False

setup()
