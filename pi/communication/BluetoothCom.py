from queue import Queue
from serial import Serial
from threading import Thread

class BluetoothCom(Thread):
    def __init__(self, port, baudRate, timeout, handlers):
        super(BluetoothCom, self).__init__()

        self.port = port
        self.baudRate = baudRate
        self.timeout = timeout
        self._sendQueue = Queue()
        self.handlers = handlers
        self.isRunning = True

    def start(self):
        self.stream = Serial(self.port, baudrate=self.baudRate, timeout=self.timeout)
        super(BluetoothCom, self).start()
        
    def close(self):
        self.isRunning = False
        
    def send(self, *msg):
        msgAsStr = ",".join(msg)
        self._sendQueue.put(msgAsStr+";")

    def run(self):
        while self.isRunning:
            if not self._sendQueue.empty():
                cmd = self._sendQueue.get()
                self.stream.write(cmd.encode())
                
            val = str(self.stream.readline().decode().strip(';'))
            
            if(val == ""):
                continue
            
            cmd = val.split(",")
            if(len(cmd) == 0):
                continue
            
            opCode = cmd[0]
            if(opCode in self.handlers):
                self.handlers[opCode](cmd[1:])
            else:
                self.send("error", "Handler invalido")
        self.stream.close()
