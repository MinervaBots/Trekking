from threading import Thread
import PyCmdMessenger
import queue

class CommandMessager(Thread):
    def __init__(self, arduino, commands, defaulHandler):
        super(CommandMessager, self).__init__()
        self._commands = list(commands)
        self._cmdMessenger = PyCmdMessenger.CmdMessenger(arduino, commands)
        self.sendQueue = queue.Queue()
        self.defaulHandler = defaulHandler
        self.isRunning = True

    def stop(self):
        self.isRunning = False
        
    def run(self):
        while self.isRunning:
            if not self.sendQueue.empty():
                cmd = self.sendQueue.get()
                self._cmdMessenger.send(cmd[0], *cmd[1])
            msg = self._cmdMessenger.receive()
            self.handle(msg)

    def send(self, cmd, *args):
        self.sendQueue.put([cmd, args])
        
    def handle(self, msg):
        if msg is None:
            return
        
        args = msg[1:]
        for command in self._commands:
            if command[0] == msg[0]:
                command[2](*args)
                return
        defaulHandler(*args)
