from queue import Queue
from serial import Serial
from threading import Thread
from typing import List, Dict, Any
from .MessageHandler import MessageHandler
from .CommonMessageCodes import CommonMessageCodes
import io

class MessagingThread(Thread):
    HandlerCollection = Dict[int, List[MessageHandler]]
    
    def __init__(self, handlers : List[MessageHandler]):
        super(MessagingThread, self).__init__()
        self.__sendQueue = Queue()
        
        self.__handlers = {}
        for handler in handlers:
            opCode = int(handler.opCode)
            if(opCode not in self.__handlers):
                self.__handlers[opCode] = list()
            self.__handlers[opCode].append(handler)
        
        self.setPort("")
        self.setOptions(9600, 0.1, 1000);
        self.setSeparators(",", ";")
        self.isRunning = False
        
    def setPort(self, port):
        self.port = port
        
    def setOptions(self, baudRate : int = 9600, timeout : float = 0.1, bufferQueueSize : int = 1000):
        self.baudRate = baudRate
        self.timeout = timeout
        self.bufferQueueSize = bufferQueueSize
        
    def setSeparators(self, fieldSeparator : str, messageSeparator : str):
        self.fieldSeparator = fieldSeparator
        self.messageSeparator = messageSeparator
        
    def start(self):
        if self.port == "":
            raise ValueError("A porta deve ser definida antes de iniciar a thread")
            
        self.stream = Serial(self.port, baudrate=self.baudRate, timeout=self.timeout)
        self.isRunning = True
        super(MessagingThread, self).start()
        
    def close(self):
        self.isRunning = False
        
    def send(self, opCode, *args):
        message = str(int(opCode)) + self.fieldSeparator + self.fieldSeparator.join(str(field) for field in args) + self.messageSeparator
        self.__sendQueue.put(message.encode())

    def run(self):
        while self.isRunning and self.stream.isOpen():
            # Primeiro envia as mensagens
            if not self.__sendQueue.empty():
                self.stream.write(self.__sendQueue.get())
               
            decodedBuffer : str = ""
            while True:
                oneByte = self.stream.read(1).decode()
                if oneByte == self.messageSeparator:
                    if(decodedBuffer != ""):
                        break
                else:
                    decodedBuffer += oneByte

            message = decodedBuffer.split(self.fieldSeparator)
            if(len(message) == 0):
                continue
            
            opCode = int(message[0])
            if(opCode in self.__handlers):
                for handler in self.__handlers[opCode]:
                    handler.handle(self, message[1:])
            else:
                self.send(CommonMessageCodes.ERROR, "Handler invalido")
        
        # Só fecha a stream quando sair do loop
        # Evita erros devido a thread não ser sincronizada
        self.stream.close()
