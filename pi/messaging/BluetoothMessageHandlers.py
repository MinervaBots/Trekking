import dic
from abc import *
from typing import List
from enum import IntEnum, auto
from .MessageHandler import *
from .CommonMessageCodes import CommonMessageCodes
from tracking.Tracker import Tracker

from SystemInfo import SystemInfo


class BluetoothMessageCodes(IntEnum):
    START_SYSTEM = 100
    STOP_SYSTEM = auto()
    SET_TRACKING_METHOD = auto()

	
class BluetoothMessageHandlersModule(dic.container.Module):
    def load(self, builder):
        builder.register_class(StartSystemHandler, register_as = [BluetoothMessageHandler], component_scope=dic.scope.SingleInstance)
        builder.register_class(StopSystemHandler, register_as = [BluetoothMessageHandler], component_scope=dic.scope.SingleInstance)
        builder.register_class(SetTrackingMethodHandler, register_as = [BluetoothMessageHandler], component_scope=dic.scope.SingleInstance)

		
class BluetoothMessageHandler(MessageHandler, ABC):
    def __init__(self):
        self.opCode = 0
    
    @abstractmethod
    def handle(self, message : List):
        raise NotImplementedError

    
class StartSystemHandler(BluetoothMessageHandler):
    def __init__(self, systemInfo : SystemInfo):
        self.opCode = BluetoothMessageCodes.START_SYSTEM
        self.__systemInfo = systemInfo
        
    def handle(self, message):
        self.__systemInfo.isRunning = True
        
class StopSystemHandler(BluetoothMessageHandler):
    def __init__(self, systemInfo : SystemInfo):
        self.opCode = BluetoothMessageCodes.STOP_SYSTEM
        self.__systemInfo = systemInfo
        
    def handle(self, message):
        self.__systemInfo.isRunning = False
        
class SetTrackingMethodHandler(BluetoothMessageHandler):
    def __init__(self, tracker : Tracker):
        self.opCode = BluetoothMessageCodes.SET_TRACKING_METHOD
        self.__tracker = tracker
    
    def handle(self, message):
        self.__tracker.setTrackingMethod(message[0])