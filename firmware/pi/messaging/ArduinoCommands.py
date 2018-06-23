import dic
from abc import *
from typing import List
from enum import IntEnum, auto
from .MessageHandler import *
from .MessageCodes import MessageCodes
from .BluetoothMessagingThread import BluetoothMessagingThread
from .ArduinoMessageHandlers import ArduinoMessageHandler
from .MessagingThread import MessagingThread

class ArduinoCommands(ABC):
  def __init__(self):
    self.list = []
    self.list.append([MessageCodes.TARGET_FOUND, "fi"])
    self.list.append([MessageCodes.TARGET_LOST, ""])
    self.list.append([MessageCodes.STOP_EVENT, ""])
    