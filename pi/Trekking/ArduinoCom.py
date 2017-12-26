from threading import Thread
import PyCmdMessenger
import queue
import datetime

class ArduinoCom(Thread):
    def __init__(self, port, baudRate, commands):
        super(ArduinoCom, self).__init__()

        self._arduino = PyCmdMessenger.ArduinoBoard(port, baud_rate = baudRate, settle_time = 0.5)
        self._cmdMessenger = PyCmdMessenger.CmdMessenger(self._arduino, commands)
        
        self._commands = list(commands)
        self._sendQueue = queue.Queue()
        self.isRunning = True

    def close(self):
        self.isRunning = False
        
    def run(self):
        while self.isRunning:
            if not self._sendQueue.empty():
                cmd = self._sendQueue.get()
                self._cmdMessenger.send(cmd[0], *cmd[1])
            msg = self._cmdMessenger.receive()
            self.handle(msg)
        
        self._arduino.close()
        #self._cmdMessenger.close()
            
    def send(self, cmd, *args):
        self._sendQueue.put([cmd, args])
        
    def handle(self, msg):
        if msg is None:
            return
        for command in self._commands:
            if command[0] == msg[0]:
                args = msg[1:]
                command[2](*args)
                return
