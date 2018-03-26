import serial
from time import sleep
import sys
import queue
from threading import Thread
from sys import platform as _platform


isRaspberryPi = "linux" in _platform

port = "/dev/rfcomm1" if isRaspberryPi else "COM4"
baud = 9600
ser = serial.Serial(port, baudrate=baud, timeout=0.1)

sendQueue = queue.Queue()


def exitHandler(cmd):
    ser.close()
    sys.exit(0)


handlers = {"exit": exitHandler}

def sendCommand(*cmd):
    cmdAsStr = "/".join(cmd)
    sendQueue.put(cmdAsStr+"\r\n")

    
def recvLoop():
    while ser.is_open:
        val = str(ser.readline().decode().strip('\r\n'))
        if(val == ""):
            continue
        
        cmd = val.split("/")
        if(len(cmd) == 0):
            continue
        
        opCode = cmd[0]
        if(opCode in handlers):
            handlers[opCode](cmd)
        else:
            sendCommand("error", "Handler invalido")

def sendLoop():
    while ser.is_open:
        if not sendQueue.empty():
            cmd = sendQueue.get()
            ser.write(cmd.encode())
            
recvThread = Thread(target = recvLoop, args = ())
sendThread = Thread(target = sendLoop, args = ())

recvThread.start()
sendThread.start()

while ser.is_open:
    s = input("Envie algo")
    sendCommand(s)
