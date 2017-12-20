from utils.FPS import FPS
from videoStream.VideoStream import VideoStream
import cv2
import time
import PyCmdMessenger

#Configurações da imagem
showWindows = True
windowName = "cameraFeed"
frameWidth = 640
frameHeight = 368
frameCenterX = frameWidth / 2

#PyCmdMessage
arduino = PyCmdMessenger.ArduinoBoard("COM3", baud_rate = 9600)
commands = [["targetData", "fiiii"],
            ["error", "s"]]

#Medida de performance
fps = FPS().start()

#Captura de video
vs = VideoStream(usePiCamera=True, resolution=(frameWidth, frameHeight)).start()
time.sleep(2.0) #Usando a Câmera do Raspberry Pi precisamos dar um tempinho pra ela "esquentar"

#cascadeDetector = cv2.CascadeClassifier('/home//pi//Trekking//Code//Pi//Full//Python//haarCascade.xml')
cascadeDetector = cv2.CascadeClassifier('haarCascades/face.xml')

#Só inicializa a janela se solicitarmos
if showWindows:
    cv2.namedWindow(windowName)
    cv2.resizeWindow(windowName, frameWidth, frameHeight)
    cv2.startWindowThread()

try:
    while True:
        frame = vs.read()
        if frame is None:
            print('Unable to grab from the camera')
            break
        
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        objects = cascadeDetector.detectMultiScale(gray, 1.3, 5)
        if len(objects) > 0:
            (x,y,w,h) = objects[0]
            centerX = (x + w) / 2.0
            direction = (frameCenterX / centerX) - 1
            PyCmdMessenger.send("targetData", direction, x, y, w, h)
            
        if showWindows:
            if(len(objects) > 0):
                (x,y,w,h) = objects[0]
                cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,0),2)
            cv2.imshow(windowName, frame)
            if cv2.waitKey(1) == 27:
                break  # esc to quit
        fps.update()
        print("FPS: {:.2f}".format(fps.fps()))
finally:
    fps.stop()
    print("[INFO] elasped time: {:.2f}".format(fps.elapsed()))
    print("[INFO] Mean. FPS: {:.2f}".format(fps.meanFps()))
    # do a bit of cleanup
    cv2.destroyAllWindows()
    vs.stop()
