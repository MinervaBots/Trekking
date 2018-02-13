import abc
from enum import IntEnum
from typing import List
from .MessageHandler import MessageHandler
from .CommonMessageCodes import CommonMessageCodes
from .ArduinoMessageHandlers import ArduinoMessageHandler
from .BluetoothMessageHandlers import BluetoothMessageHandler
from tracking.Tracker import Tracker

class StartTrackerHandler(ArduinoMessageHandler, BluetoothMessageHandler):
    def __init__(self, tracker : Tracker):
        self.opCode = CommonMessageCodes.START_TRACKING
        self.__tracker = tracker
        
    def handle(self, message):
        self.__tracker.start()
        
class PauseTrackerHandler(ArduinoMessageHandler, BluetoothMessageHandler):
    def __init__(self, tracker : Tracker):
        self.opCode = CommonMessageCodes.PAUSE_TRACKING
        self.__tracker = tracker
    
    def handle(self, message):
        self.__tracker.pause()