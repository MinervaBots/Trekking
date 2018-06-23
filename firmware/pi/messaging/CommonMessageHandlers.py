import abc
from enum import IntEnum
from typing import List
from .MessageHandler import MessageHandler
from .MessageCodes import MessageCodes
from .ArduinoMessageHandlers import ArduinoMessageHandler
from .BluetoothMessageHandlers import BluetoothMessageHandler
from tracking.Tracker import Tracker
from .MessagingThread import MessagingThread

class StartTrackerHandler(ArduinoMessageHandler, BluetoothMessageHandler):
    def __init__(self, tracker : Tracker):
        self.opCode = MessageCodes.START_TRACKING
        self.__tracker = tracker
        self.parametersTypes = ""
        
    def handle(self, sender : MessagingThread, message):
        self.__tracker.start()
        
class PauseTrackerHandler(ArduinoMessageHandler, BluetoothMessageHandler):
    def __init__(self, tracker : Tracker):
        self.opCode = MessageCodes.PAUSE_TRACKING
        self.__tracker = tracker
        self.parametersTypes = ""
    
    def handle(self, sender : MessagingThread, message):
        self.__tracker.pause()