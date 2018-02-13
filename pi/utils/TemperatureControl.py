import os
import time
from sys import platform as _platform

class TemperatureControl:
    def __init__(self, delay):
        self.delay = delay
        self.lastMeasure = time.time()
        self._temp = os.popen("vcgencmd measure_temp").readline()
        self._isWindows = isRaspberryPi = "linux" not in _platform

    def update(self, log = False):
        if self._isWindows:
            return -1.0
            
        if time.time() - self.lastMeasure > self.delay:
            self._temp = os.popen("vcgencmd measure_temp").readline()
            self.lastMeasure = time.time()
            
        self.tempCelcius = float(self._temp.replace("temp=", "").replace("'C", ""))
        if log:
            print("[INFO]: " + self._temp)
        
        return self.tempCelcius
