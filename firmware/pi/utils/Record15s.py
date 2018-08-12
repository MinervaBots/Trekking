import io
import picamera
import time
import subprocess
import os

stream = io.BytesIO()
with picamera.PiCamera() as camera:
    camera.resolution = (640, 360)
    fileName = time.time()
    camera.start_recording("{}.h264".format(fileName))
    camera.wait_recording(5 * 60)
    camera.stop_recording()
    
    command = "MP4Box -add {}.h264 {}.mp4".format(fileName, fileName)
    try:
        output = subprocess.check_output(command, stderr=subprocess.STDOUT, shell=True)
        os.remove("{}.h264".format(fileName))
        
    except subprocess.CalledProcessError as e:
        print('FAIL:\ncmd:{}\noutput:{}'.format(e.cmd, e.output))