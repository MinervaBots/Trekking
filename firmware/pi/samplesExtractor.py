from videoStream.FileVideoStream import FileVideoStream
import cv2
import numpy as np
from datetime import datetime
import os
import random
import tkinter as tk
from tkinter import filedialog
import sys
from random import randint


root = tk.Tk()
root.withdraw()

videoPath = filedialog.askopenfilename()
samplesPath = filedialog.askdirectory()

 
trackerTypes = ['BOOSTING', 'MIL', 'KCF','TLD', 'MEDIANFLOW', 'GOTURN', 'MOSSE', 'CSRT']

def createTrackerByName(trackerType):
  # Create a tracker based on tracker name
  if trackerType == trackerTypes[0]:
    tracker = cv2.TrackerBoosting_create()
  elif trackerType == trackerTypes[1]: 
    tracker = cv2.TrackerMIL_create()
  elif trackerType == trackerTypes[2]:
    tracker = cv2.TrackerKCF_create()
  elif trackerType == trackerTypes[3]:
    tracker = cv2.TrackerTLD_create()
  elif trackerType == trackerTypes[4]:
    tracker = cv2.TrackerMedianFlow_create()
  elif trackerType == trackerTypes[5]:
    tracker = cv2.TrackerGOTURN_create()
  elif trackerType == trackerTypes[6]:
    tracker = cv2.TrackerMOSSE_create()
  elif trackerType == trackerTypes[7]:
    tracker = cv2.TrackerCSRT_create()
  else:
    tracker = None
    print('Incorrect tracker name')
    print('Available trackers are:')
    for t in trackerTypes:
      print(t)
  return tracker


selectedRegionId = -1


def redrawRegions(frame):
  tempFrame = frame.copy()
  for i, box in enumerate(bboxes):
    p1 = (int(box[0]), int(box[1]))
    p2 = (int(box[0] + box[2]), int(box[1] + box[3]))
    cv2.rectangle(tempFrame, p1, p2, colors[i], 2, 1)

    if (i == selectedRegionId):
      p3 = (int(box[0]), int(box[1]))
      p4 = (int(box[0] + box[2]), int(box[1] + box[3]))
      cv2.line(tempFrame, p3, p4, colors[selectedRegionId], 2, 1)
      cv2.imshow("Foreground", tempFrame)
  return tempFrame

def rebuildTracker(frame):
  global multiTracker
  multiTracker = cv2.MultiTracker_create()
  for i, box in enumerate(bboxes):
    bbox = tuple(box)
    multiTracker.add(createTrackerByName(trackerType), frame, bbox)
  
def selectRegions(frame):
    global selectedRegionId

    cv2.imshow("Foreground", redrawRegions(frame))
    
    boxes = []
    # OpenCV's selectROI function doesn't work for selecting multiple objects in Python
    # So we will call this function in a loop till we are done selecting all objects
    while True:
        tempFrame = redrawRegions(frame)
        cv2.imshow("Foreground", tempFrame)
        k = cv2.waitKey(0)
        
        if (k == ord('a')):
          selectedRegionId -= 1
          if (selectedRegionId < 0):
            selectedRegionId = len(bboxes) - 1

        elif (k == ord('d')):
          selectedRegionId += 1
          if (selectedRegionId > len(bboxes) - 1):
            selectedRegionId = 0
          
        elif (k == 8): # Backspace
          if (selectedRegionId != -1):
            del bboxes[selectedRegionId]
            del colors[selectedRegionId]
            selectedRegionId = -1
            rebuildTracker(frame)
            
        elif (k == ord('+')):
          bbox = cv2.selectROI('Foreground', tempFrame)
          if(bbox != (0, 0, 0, 0)):
            bboxes.append(bbox)
            colors.append((randint(0, 255), randint(0, 255), randint(0, 255)))
            multiTracker.add(createTrackerByName(trackerType), frame, bbox)
            
        elif k == ord('q'):
          break


def calculateIntersection(a0, a1, b0, b1):
    if a0 >= b0 and a1 <= b1: # Contained
        intersection = a1 - a0
    elif a0 < b0 and a1 > b1: # Contains
        intersection = b1 - b0
    elif a0 < b0 and a1 > b0: # Intersects right
        intersection = a1 - b0
    elif a1 > b1 and a0 < b1: # Intersects left
        intersection = b1 - a0
    else: # No intersection (either side)
        intersection = 0

    return intersection

countPos = 0
countNeg = 0
def saveSamples(frame, positives, negatives):
  global countPos, countNeg
  
  for rect in positives:
        # 33% de chance de pular as amostras negativas dessa imagem
        # A ideia é evitar também muitas imagens de positivo parecidas
        if (random.randint(0, 2) == 0):
          continue
        
        x,y,w,h = map(int, rect)
        roi = frame[y:y+h, x:x+w]

        # Evita substituir arquivos já existentes
        while(os.path.isfile(samplesPath + "/p/%d.jpg" % countPos)):
            countPos += 1

        # Salva a região encontrada num .jpg
        cv2.imwrite(samplesPath + "/p/%d.jpg" % countPos, roi)
        countPos += 1


  for rect in negatives:  
      # 66% de chance de pular as amostras negativas dessa imagem
      # A ideia é evitar também muitos backgrounds parecidos
      if (random.randint(0, 2) != 0):
        continue
  
      x,y,w,h = map(int, rect)
      roi = frame[y:y+h, x:x+w]
      
      # Evita substituir arquivos já existentes
      while(os.path.isfile(samplesPath + "/n/%d.jpg" % countNeg)):
          countNeg += 1

      # Salva a região encontrada num .jpg
      cv2.imwrite(samplesPath + "/n/%d.jpg" % countNeg, roi)
      countNeg += 1
      
  
video = FileVideoStream(videoPath, resolution = (640, 368))
video.start()
frame = video.read()
frame = cv2.resize(frame, (640, 368))


cv2.namedWindow("Foreground", cv2.WINDOW_AUTOSIZE)

## Select boxes
bboxes = []
colors = [] 

# Specify the tracker type
trackerType = "CSRT"
 
# Create MultiTracker object
multiTracker = cv2.MultiTracker_create()


selectRegions(frame)
frameHeight, frameWidth, _ = frame.shape
  
while video.running():
    originalFrame = frame.copy()

    # get updated location of objects in subsequent frames
    success, positives = multiTracker.update(frame)
 
    # draw tracked objects
    for i, newbox in enumerate(positives):
        p1 = (int(newbox[0]), int(newbox[1]))
        p2 = (int(newbox[0] + newbox[2]), int(newbox[1] + newbox[3]))
        cv2.rectangle(frame, p1, p2, colors[i], 2, 1)
        bboxes[i] = newbox


    negatives = []
    # Pega regioes sem cones para servir de negativas

    
    x, y, w, h = 0, 0, int(round(frameWidth * 0.25)), int(round(frameHeight * 0.25))
    while (x < frameWidth):
        y = 0
        while (y < frameHeight):
            add = True
            rect = [x, y, w, h]
            for pos in positives:
                interWidth = calculateIntersection(rect[0], rect[0] + rect[2], pos[0], pos[0] + pos[2])
                interHeight = calculateIntersection(rect[1], rect[1] + rect[3], pos[1], pos[1] + pos[3])
                
                if (interWidth != 0 and interHeight != 0):
                    add = False
                    break
            if add:
                cv2.rectangle(frame,(x, y),(x + w, y + h), (0, 0, 255), 2)
                negatives.append(rect)
            y += h
        x += w

    
    #_, positives, negatives = detector.detect(frame)
    cv2.imshow("Foreground", frame)
    saveSamples(originalFrame, positives, negatives)
    
    frame = video.read()
    frame = cv2.resize(frame, (640, 368))

    key = cv2.waitKey(1) & 0xFF

    if key == 113:
        selectRegions(frame)
    elif key == 27:  # Esc pressed
        break

cv2.destroyAllWindows()
