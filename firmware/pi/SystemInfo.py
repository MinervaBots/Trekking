import sys
import os

class SystemInfo:
    def __init__(self):
        self.isRunning = False
        self.isRaspberryPi = "linux" in sys.platform
        
        if(self.isRaspberryPi):
            self.arduinoPort = "/dev/ttyACM0"
            self.bluetoothPort = "/dev/rfcomm0"
            self.enableWindow = "DISPLAY" in os.environ
        else:
            self.arduinoPort = "COM3"
            self.bluetoothPort = "COM4"
            self.enableWindow = True
            
        self.isTracking = False
        self.trackedRects = [(0, 0, 0, 0)]
        self.trackedDirections = [0]