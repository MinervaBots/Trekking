import dic
from abc import *
from typing import List
from enum import IntEnum
from .MessageHandler import *
from .MessageCodes import MessageCodes
from .BluetoothMessagingThread import BluetoothMessagingThread
from .MessagingThread import MessagingThread

class ArduinoMessageHandlersModule(dic.container.Module):
  def load(self, builder):
    builder.register_class(ArduinoLogMessageHandler, register_as = [ArduinoMessageHandler], component_scope=dic.scope.SingleInstance)
    pass
  
class ArduinoMessageHandler(MessageHandler, ABC):
  def __init__(self):
    self.opCode = 0
    self.parametersTypes = ""
  
  @abstractmethod
  def handle(self, sender : MessagingThread, message : List):
    raise NotImplementedError
    
class ArduinoLogMessageHandler(ArduinoMessageHandler):
  def __init__(self, bluetoothMessager : BluetoothMessagingThread):
    self.opCode = MessageCodes.LOG
    self.__bluetoothMessager = bluetoothMessager
    self.parametersTypes = "s*"
    
  def handle(self, sender : MessagingThread, message : List):
    print("Log: " + str(message))
    #self.__bluetoothMessager.send(*message)
    pass