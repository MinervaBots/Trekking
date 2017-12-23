from threading import Thread
import PyCmdMessenger
import queue
import datetime

class CommandMessager(Thread):

    def __init__(self, arduino, commands, lock):
        super(CommandMessager, self).__init__()
        self._commands = list(commands)
        self._cmdMessenger = PyCmdMessenger.CmdMessenger(arduino, commands)
        self.sendQueue = queue.Queue()
        self.lock = lock
        self.isRunning = True

    def stop(self):
        self.isRunning = False
        
    def run(self):
        while self.isRunning:
            if not self.sendQueue.empty():
                cmd = self.sendQueue.get()
                #print(cmd)
                self._cmdMessenger.send(cmd[0], *cmd[1])
            #start = datetime.datetime.now()
            msg = self._cmdMessenger.receive()
            self.handle(msg)
            #end = datetime.datetime.now()
            #delta = (end - start).total_seconds() * 1000
            #print(delta)

    def send(self, cmd, *args):
        self.sendQueue.put([cmd, args])
        
    def handle(self, msg):
        if msg is None:
            return
        for command in self._commands:
            if command[0] == msg[0]:
                args = msg[1:]
                #print(args)
                #print(datetime.datetime.now())
                command[2](*args)
                return
