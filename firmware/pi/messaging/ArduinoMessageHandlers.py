import dic
from abc import *
from typing import List
from enum import IntEnum, auto
from .MessageHandler import *
from .CommonMessageCodes import CommonMessageCodes
from .BluetoothMessagingThread import BluetoothMessagingThread
from .MessagingThread import MessagingThread

class ArduinoMessageCodes(IntEnum):
  LOG = 0
  TARGET_FOUND = auto()
  TARGET_LOST = auto()
  STOP_EVENT = 5

class ArduinoMessageHandlersModule(dic.container.Module):
  def load(self, builder):
    builder.register_class(ArduinoLogMessageHandler, register_as = [ArduinoMessageHandler], component_scope=dic.scope.SingleInstance)
    pass
  
class ArduinoMessageHandler(MessageHandler, ABC):
  def __init__(self):
    self.opCode = 0
  
  @abstractmethod
  def handle(self, sender : MessagingThread, message : List):
    raise NotImplementedError
    
class ArduinoLogMessageHandler(ArduinoMessageHandler):
  def __init__(self, bluetoothMessager : BluetoothMessagingThread):
    self.opCode = ArduinoMessageCodes.LOG
    self.__bluetoothMessager = bluetoothMessager
    
  def handle(self, sender : MessagingThread, message : List):
    print("Log: " + str(message))
    #self.__bluetoothMessager.send(*message)
    pass