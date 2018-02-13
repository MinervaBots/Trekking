from abc import *
from typing import List

class MessageHandler(ABC):
	def __init__(self):
		self.opCode = 0
	
	@abstractmethod
	def handle(self, message : List):
		raise NotImplementedError