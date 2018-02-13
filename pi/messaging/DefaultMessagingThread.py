from queue import Queue
from serial import Serial
from threading import Thread
from typing import List
from .MessageHandler import MessageHandler
from .CommonMessageCodes import CommonMessageCodes

class DefaultMessagingThread(Thread):
    def __init__(self, handlers : List[MessageHandler]):
        super(DefaultMessagingThread, self).__init__()
        self.__sendQueue = Queue()
        
        self.__handlers = {}
        for handler in handlers:
            opCode = int(handler.opCode)
            #print(opCode)
            self.__handlers[opCode] = handler
        
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
        super(DefaultMessagingThread, self).start()
        
    def close(self):
        self.isRunning = False
        
    def send(self, opCode, *args):
        message = str(int(opCode)) + self.fieldSeparator + self.fieldSeparator.join(args) + self.messageSeparator
        self.__sendQueue.put(message.encode())

    def run(self):
        while self.isRunning:
            # Primeiro envia as mensagens
            if not self.__sendQueue.empty():
                self.stream.write(self.__sendQueue.get())
                
            streamInput = str(self.stream.readline().decode().strip(self.messageSeparator))
            if(streamInput == ""):
                continue
            
            message = streamInput.split(self.fieldSeparator)
            if(len(message) == 0):
                continue
            
            opCode = int(message[0])
            #print(opCode)
            if(opCode in self.__handlers):
                self.__handlers[opCode].handle(message[1:])
            else:
                self.send(CommonMessageCodes.ERROR, "Handler invalido")
        
        # Só fecha a stream quando sair do loop
        # Evita erros devido a thread não ser sincronizada
        self.stream.close()
