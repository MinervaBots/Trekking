import dic
from abc import *
from typing import List
from enum import IntEnum, auto
from .MessageHandler import *
from .CommonMessageCodes import CommonMessageCodes
from tracking.Tracker import Tracker
from utils.FPS import FPS
from utils.TemperatureControl import TemperatureControl
from SystemInfo import SystemInfo
from .MessagingThread import MessagingThread

class BluetoothMessageCodes(IntEnum):
    START_SYSTEM = 100
    STOP_SYSTEM = auto()
    SET_TRACKING_METHOD = auto()
    GET_FPS = auto()
    GET_TEMPERATURE = auto()

	
class BluetoothMessageHandlersModule(dic.container.Module):
    def load(self, builder):
        builder.register_class(StartSystemHandler, register_as = [BluetoothMessageHandler], component_scope=dic.scope.SingleInstance)
        builder.register_class(StopSystemHandler, register_as = [BluetoothMessageHandler], component_scope=dic.scope.SingleInstance)
        builder.register_class(SetTrackingMethodHandler, register_as = [BluetoothMessageHandler], component_scope=dic.scope.SingleInstance)
        builder.register_class(GetFPSHandler, register_as = [BluetoothMessageHandler], component_scope=dic.scope.SingleInstance)
        builder.register_class(GetTemperatureHandler, register_as = [BluetoothMessageHandler], component_scope=dic.scope.SingleInstance)

		
class BluetoothMessageHandler(MessageHandler, ABC):
    def __init__(self):
        self.opCode = 0
    
    @abstractmethod
    def handle(self, sender : MessagingThread, message : List):
        raise NotImplementedError

    
class StartSystemHandler(BluetoothMessageHandler):
    def __init__(self, systemInfo : SystemInfo):
        self.opCode = BluetoothMessageCodes.START_SYSTEM
        self.__systemInfo = systemInfo
        
    def handle(self, sender : MessagingThread, message : List):
        self.__systemInfo.isRunning = True
        
class StopSystemHandler(BluetoothMessageHandler):
    def __init__(self, systemInfo : SystemInfo):
        self.opCode = BluetoothMessageCodes.STOP_SYSTEM
        self.__systemInfo = systemInfo
        
    def handle(self, sender : MessagingThread, message : List):
        self.__systemInfo.isRunning = False
        
class SetTrackingMethodHandler(BluetoothMessageHandler):
    def __init__(self, tracker : Tracker):
        self.opCode = BluetoothMessageCodes.SET_TRACKING_METHOD
        self.__tracker = tracker
    
    def handle(self, sender : MessagingThread, message : List):
        self.__tracker.setTrackingMethod(message[0])
        
class GetFPSHandler(BluetoothMessageHandler):
    def __init__(self, fps : FPS):
        self.opCode = BluetoothMessageCodes.GET_FPS
        self.__fps = fps
    
    def handle(self, sender : MessagingThread, message : List):
        sender.send(self.opCode, "FPS: {:.2f}".format(self.__fps.lastFps()))
        
class GetTemperatureHandler(BluetoothMessageHandler):
    def __init__(self, temp : TemperatureControl):
        self.opCode = BluetoothMessageCodes.GET_TEMPERATURE
        self.__temp = temp
    
    def handle(self, sender : MessagingThread, message : List):
        sender.send(self.opCode, "Temp: {:.2f} 'C".format(self.__temp.update()))