import dic
from abc import *
from typing import List
from enum import IntEnum, auto
from .MessageHandler import *
from .CommonMessageCodes import CommonMessageCodes
from .BluetoothMessagingThread import BluetoothMessagingThread

class ArduinoMessageCodes(IntEnum):
	LOG = 400
	MPU_DATA = auto()
	TARGET_DATA = auto() # *


class ArduinoMessageHandlersModule(dic.container.Module):
    def load(self, builder):
        builder.register_class(ArduinoLogMessageHandler, register_as = [ArduinoMessageHandler], component_scope=dic.scope.SingleInstance)

	
class ArduinoMessageHandler(MessageHandler, ABC):
	def __init__(self):
		self.opCode = 0
	
	@abstractmethod
	def handle(self, message : List):
		raise NotImplementedError
		
class ArduinoLogMessageHandler(ArduinoMessageHandler):
	def __init__(self, bluetoothMessager : BluetoothMessagingThread):
		self.opCode = ArduinoMessageCodes.LOG
		self.__bluetoothMessager = bluetoothMessager
		
	def handle(self, message : List):
		print(message)
		self.__bluetoothMessager.send(message[0], message[1:])