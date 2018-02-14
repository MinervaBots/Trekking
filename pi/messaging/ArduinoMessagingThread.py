from queue import Queue
from serial import Serial
from typing import List
from threading import Thread
from .MessageHandler import MessageHandler
from .MessagingThread import MessagingThread
from .ArduinoMessageHandlers import ArduinoMessageHandler

class ArduinoMessagingThread(MessagingThread):
    def __init__(self, handlers : List[ArduinoMessageHandler]):
        super(ArduinoMessagingThread, self).__init__(handlers)