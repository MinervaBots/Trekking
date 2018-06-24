from queue import Queue
from serial import Serial
from typing import List
from threading import Thread
from .MessageHandler import MessageHandler
from .MessagingThread import MessagingThread
from .BluetoothMessageHandlers import BluetoothMessageHandler

class BluetoothMessagingThread(MessagingThread):
    def __init__(self, 	handlers : List[BluetoothMessageHandler]):
        super(BluetoothMessagingThread, self).__init__(handlers)