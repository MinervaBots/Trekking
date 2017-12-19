from VideoStream import *
from FPS import *
import cv2
import time


fps = FPS().start()
vs = VideoStream(usePiCamera=True, resolution=(640, 368)).start()
time.sleep(2.0)

cascadeDetector = cv2.CascadeClassifier('/home//pi//Trekking//Code//Pi//Full//Python//haarCascade.xml')

#cv2.namedWindow('Live');
try:
    while True:
        # grab the frame from the threaded video stream and resize it
        # to have a maximum width of 400 pixels
        frame = vs.read()
        if frame is None:
            print('Unable to grab from the camera')
            break
        
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        objects = cascadeDetector.detectMultiScale(gray, 1.3, 5)
        #for (x,y,w,h) in objects:
            #cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,0),2)
        
        #cv2.imshow('Live', frame)
        fps.update()
	print("FPS: {:.2f}".format(fps.fps()))
except KeyboardInterrupt:
    fps.stop()
    print("[INFO] elasped time: {:.2f}".format(fps.elapsed()))
    print("[INFO] Mean. FPS: {:.2f}".format(fps.meanFps()))

    # do a bit of cleanup
    #cv2.destroyAllWindows()
    vs.stop()

