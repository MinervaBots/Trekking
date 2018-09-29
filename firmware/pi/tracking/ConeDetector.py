import cv2
import numpy as np

class ConeDetector:
    orangeRange1 = [(0, 80, 100), (25, 255, 255)]
    orangeRange2 = [(170, 80, 100), (180, 255, 255)]
    whiteRange = [(0, 0, 120), (180, 30, 255)]
    
    def calculateIntersection(self, a0, a1, b0, b1):
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
    
    def scale(self, rect, factor):
        rect = list(rect)
        w_temp = rect[2]
        h_temp = rect[3]
        rect[1] = rect[1] - h_temp * factor
        rect[0] = rect[0] - w_temp * factor
        rect[2] = w_temp * (1 + factor * 2)
        rect[3] = h_temp * (1 + factor * 2)
        return rect
    
    def rectContains(self, rect, pt):
        logic = rect[0] < pt[0] < rect[0]+rect[2] and rect[1] < pt[1] < rect[1]+rect[3]
        return logic
    
    def detect(self, frame):
        frame = frame.copy()
        frameHeight, frameWidth, _ = frame.shape
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        
        orange1 = self.detectColor(hsv, ConeDetector.orangeRange1)
        orange2 = self.detectColor(hsv, ConeDetector.orangeRange2)
        orange = orange1 + orange2
        #orange = cv2.convertScaleAbs(orange)

        #_, contours, _ = cv2.findContours(orange, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        #cv2.drawContours(orange, [contour for contour in contours if not(200 < cv2.contourArea(contour) < 10000)], -1, (0,0,0), -1)
        
        white = self.detectColor(hsv, ConeDetector.whiteRange)
        white = cv2.convertScaleAbs(white)
        _, contours, _ = cv2.findContours(white, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        cv2.drawContours(white, [contour for contour in contours if not(200 < cv2.contourArea(contour) < 5000)], -1, (0,0,0), -1)

        mask = (orange + white).astype(np.uint8)

        kernel = np.ones((3, 3), np.uint8)

        threshSmoothed = cv2.dilate(mask, kernel, iterations = 1)
        threshSmoothed = cv2.GaussianBlur(threshSmoothed, (3, 3), 0)
        
        _, contours, _ = cv2.findContours(threshSmoothed, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

        positives = []
        for contour in contours:
            hull = cv2.convexHull(contour)
            
            if not self.isConicHull(hull):
                continue
            
            rect = cv2.boundingRect(hull)
            x, y, w, h = rect
            
            hsvRect = hsv[y:y+h,x:x+w]

            orangeValue = orange[y:y+h,x:x+w].mean()
            if (orangeValue < 50):
                continue
            
            whiteValue = white[y:y+h,x:x+w].mean()
            if (whiteValue < 10):
                continue
            
            rect = list(map(int, self.scale(rect, 0.1)))
            positives.append(rect)
            
            x, y, w, h = rect
            cv2.rectangle(frame,(x,y),(x + w, y + h), (0,255,0), 2)
        
        x, y, w, h = 0, 0, int(round(frameWidth * 0.25)), int(round(frameHeight * 0.25))
        negatives = []
        # Pega regioes sem cones para servir de negativas
        
        while (x < frameWidth):
            y = 0
            while (y < frameHeight):
                add = True
                rect = [x, y, w, h]
                for pos in positives:
                    interWidth = self.calculateIntersection(rect[0], rect[0] + rect[2], pos[0], pos[0] + pos[2])
                    interHeight = self.calculateIntersection(rect[1], rect[1] + rect[3], pos[1], pos[1] + pos[3])
                    
                    if (interWidth != 0 and interHeight != 0):
                        add = False
                        break
                        
                if add:
                    cv2.rectangle(frame,(x, y),(x + w, y + h), (0, 0, 255), 2)
                    negatives.append(rect)
                y += h
                
            x += w
            
        #cv2.imshow('Objects Detected', frame)
        #cv2.waitKey(1)
        return frame, positives, negatives
    
    def isConicHull(self, convexHull):
        boundingRect = cv2.boundingRect(convexHull)
        area = boundingRect[0] * boundingRect[1]
        aspectRatio = boundingRect[2] / boundingRect[3]
        yCenter = boundingRect[1] + (boundingRect[3] / 2.0)
        
        MIN_PIXEL_WIDTH = 10
        MIN_PIXEL_HEIGHT = 10
        MAX_ASPECT_RATIO = 0.8
        MIN_PIXEL_AREA = 80
    
    
        #first do a gross dimensional check, return false if convex hull does not pass
        if (area < MIN_PIXEL_AREA or boundingRect[2] < MIN_PIXEL_WIDTH or boundingRect[3] < MIN_PIXEL_HEIGHT or aspectRatio > MAX_ASPECT_RATIO):
            return False
        
        
        #now check if the convex Hull is pointing up

        #declare and populate a vector of all points above the y center, and all points below the y center
        pointsAboveCenter = []
        pointsBelowCenter = []
        for currentPoint in convexHull:
            if (currentPoint[0][1] < yCenter):
                pointsAboveCenter.append(currentPoint[0])
            else:
                pointsBelowCenter.append(currentPoint[0])

              
        #find the left most point below the y center
        leftMostPointBelowCenter = pointsBelowCenter[0][0]
        for point in pointsBelowCenter:
            if (point[0] < leftMostPointBelowCenter): leftMostPointBelowCenter = point[0]
        

        #find the right most point below the y center
        rightMostPointBelowCenter = pointsBelowCenter[0][0]
        for point in pointsBelowCenter:
            if (point[0] > rightMostPointBelowCenter): rightMostPointBelowCenter = point[0]
        

        #step through all the points above the y center
        for pointAboveCenter in pointsAboveCenter:
            #if any point above the y center is farther left or right than the extreme left and right below y center points,: the convex hull is not pointing up, so return false            
            if (pointAboveCenter[0] <= leftMostPointBelowCenter or pointAboveCenter[0] >= rightMostPointBelowCenter):
                return False
                
        #if we get here, the convex hull has passed the gross dimensional check and the pointing up check, so we#re convinced its a cone, so return true        
        return True
        
    def detectColor(self, hsv, colorRanges):
        thresh = cv2.inRange(hsv, colorRanges[0], colorRanges[1])
        cv2.imshow('Objects Detected', thresh)
        #cv2.waitKey(0)
        #find all your connected components (white blobs in your image)
        nb_components, output, stats, centroids = cv2.connectedComponentsWithStats(thresh, connectivity=4)
        #connectedComponentswithStats yields every seperated component with information on each of them, such as size
        #the following part is just taking out the background which is also considered a component, but most of the time we don't want that.
        sizes = stats[1:, -1]; nb_components = nb_components - 1

        # minimum size of particles we want to keep (number of pixels)
        #here, it's a fixed value, but you can set it as you want, eg the mean of the sizes or whatever
        min_size = 20  

        #your answer image
        img2 = np.zeros((output.shape))
        #for every component in the image, you keep it only if it's above min_size
        for i in range(0, nb_components):
            if sizes[i] >= min_size:
                img2[output == i + 1] = 255
        
        cv2.imshow('Objects Detected', img2)
        #cv2.waitKey(0)
        return img2