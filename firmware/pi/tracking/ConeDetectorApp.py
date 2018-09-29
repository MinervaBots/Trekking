from ConeDetector import ConeDetector
import cv2
from datetime import datetime
import os
import random

startTime = datetime.now()
detector = ConeDetector()

vidcap = cv2.VideoCapture('VID_20180706_084139411.mp4')
success = vidcap.read()
countPos = 490
countNeg = 3994

# Pula um número especificado de frames para ir direto a alguma parte do video
i = 0
jumpFrames = 0
while (success and i < jumpFrames):
    success, image = vidcap.read()
    i += 1

while success:
    # Pula alguns frames em sucessão pra evitar ter amostras muito parecidas
    i = 0
    jumpFrames = random.randint(0, 7)
    while (success and i < jumpFrames):
        success, image = vidcap.read()
        i += 1
    
    _, positives, negatives = detector.detect(image)
    
    for rect in positives:
        x,y,w,h = rect
        roi = image[y:y+h, x:x+w]

        # Evita substituir arquivos já existentes
        while(os.path.isfile("samples/p/%d.jpg" % countPos)):
            countPos += 1

        # Salva a região encontrada num .jpg
        cv2.imwrite("samples/p/%d.jpg" % countPos, roi)
        countPos += 1

        
    # 50% de chance de pular as amostras negativas dessa imagem
    # A ideia é evitar também muitos backgrounds parecidos
    if (random.randint(0, 1) == 0):
        continue
    
    for rect in negatives:
        x,y,w,h = rect
        roi = image[y:y+h, x:x+w]

        # Evita substituir arquivos já existentes
        while(os.path.isfile("samples/n/%d.jpg" % countNeg)):
            countNeg += 1

        # Salva a região encontrada num .jpg
        cv2.imwrite("samples/n/%d.jpg" % countNeg, roi)
        countNeg += 1
        

elapsed = (datetime.now() - startTime).total_seconds()
print("O processamento levou: " + str(elapsed) + " segundos")
