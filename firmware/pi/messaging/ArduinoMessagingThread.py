from queue import Queue
from serial import Serial
from typing import List
from threading import Thread
from .MessageHandler import MessageHandler
from .MessagingThread import MessagingThread
from .ArduinoMessageHandlers import ArduinoMessageHandler
from .ArduinoCommands import ArduinoCommands
from .PyCmdMessenger import CmdMessenger
from .arduino import ArduinoBoard

class ArduinoMessagingThread(MessagingThread):
    def __init__(self, handlers : List[ArduinoMessageHandler], commands : ArduinoCommands):
        super(ArduinoMessagingThread, self).__init__(handlers)
        self.__commands = []
        for command in commands.list:
            command[0] = int(command[0])
            self.__commands.append(command)
        
    def start(self):
        if self.port == "":
            raise ValueError("A porta deve ser definida antes de iniciar a thread")
      
        commands = list()
        for opCode in self._handlers:
            commands.append([int(opCode), self._handlers[opCode].parametersTypes])
        for command in self.__commands:
            commands.append(command)
      
        self.__arduino = ArduinoBoard(self.port,baud_rate=self.baudRate, timeout=self.timeout)
        self.__pyCmdMessenger = CmdMessenger(self.__arduino, commands)
        self.isRunning = True
        super(MessagingThread, self).start()
    
    def send(self, opCode, *args):
        message = [int(opCode)]
        message += args
        self._sendQueue.put(message)
     
    def run(self):
        while self.isRunning and self.__arduino.connected:
            # Primeiro envia as mensagens
            if not self._sendQueue.empty():
                message = self._sendQueue.get()
                self.__pyCmdMessenger.send(*message)
        
            if(self.__arduino.comm.inWaiting() == 0):
                continue
        
            message = self.__pyCmdMessenger.receive()
            opCode = int((message[0]))
            if(opCode in self._handlers):
                self._handlers[opCode].handle(self, message)
            elif(opCode != -1):
                self.send(CommonMessageCodes.ERROR, "Handler invalido")
      
        # Só fecha a stream quando sair do loop
        # Evita erros devido a thread não ser sincronizada
        self.__arduino.close()