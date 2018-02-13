from queue import Queue
from serial import Serial
from typing import List
from threading import Thread
from .MessageHandler import MessageHandler
from .DefaultMessagingThread import DefaultMessagingThread
from .ArduinoMessageHandlers import ArduinoMessageHandler

class ArduinoMessagingThread(DefaultMessagingThread):
    def __init__(self, handlers : List[ArduinoMessageHandler]):
        super(ArduinoMessagingThread, self).__init__(handlers)