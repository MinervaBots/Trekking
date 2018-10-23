import cv2
from .Tracker import Tracker
 
class OpenCVTracker(Tracker):
    def __init__(self, detector, detectionIntervalMs, trackingMethodName = None):
        Tracker.__init__(self, detector, trackingMethodName, detectionIntervalMs)
        self.usingTracker = False
        self.__selectTracker()
        
    def setTrackingMethod(self, trackingMethodName):
        self.methodName = trackingMethodName
        self.__selectTracker()

    def init(self, frame):
        if not self.isRunning:
            return False, None, 0
            
        detected, boundingBox, direction = self._detect(frame)

        if self.usingTracker and detected:
            self.clear()
            boundingBox = tuple(boundingBox)
            return self.__tracker.init(frame, boundingBox), boundingBox, self.detector.rectToDirection(boundingBox)
            
        return detected, boundingBox, direction
    
    def _track(self, frame):
        if self.usingTracker:
            found, boundingBox = self.__tracker.update(frame)
            return found, boundingBox, self.detector.rectToDirection(boundingBox)
        else:
            return self._detect(frame)
    
    def clear(self):
        if(self.__tracker is not None):
            self.__tracker.clear()
        self.__selectTracker()

    def __selectTracker(self):
        if self.methodName == '' or self.methodName == 'CASCADE' or self.methodName == None:
            self.__tracker = None
        elif self.methodName == 'BOOSTING':
            self.__tracker = cv2.TrackerBoosting_create()
            '''
            Based on the same algorithm used to power the machine learning behind Haar cascades (AdaBoost),
            but like Haar cascades, is over a decade old. This tracker is slow and doesn’t work very well.
            Interesting only for legacy reasons and comparing other algorithms. (minimum OpenCV 3.0.0)
            '''
            
            ''' Paulo
            Extremamente lento
            '''
        elif self.methodName == 'MIL':
            self.__tracker = cv2.TrackerMIL_create()
            '''
            Better accuracy than BOOSTING tracker but does a poor job of reporting failure. (minimum OpenCV 3.0.0)
            '''
        elif self.methodName == 'KCF':
            self.__tracker = cv2.TrackerKCF_create()
            '''
            Kernelized Correlation Filters. Faster than BOOSTING and MIL.
            Similar to MIL and KCF, does not handle full occlusion well. (minimum OpenCV 3.1.0)
            '''
        elif self.methodName == 'TLD':
            self.__tracker = cv2.TrackerTLD_create()
            '''
            I’m not sure if there is a problem with the OpenCV implementation of the TLD tracker or the actual algorithm itself,
            but the TLD tracker was incredibly prone to false-positives. I do not recommend using this OpenCV object tracker. (minimum OpenCV 3.0.0)
            '''
        elif self.methodName == 'MEDIANFLOW':
            self.__tracker = cv2.TrackerMedianFlow_create()
            '''
            Does a nice job reporting failures; however, if there is too large of a jump in motion,
            such as fast moving objects, or objects that change quickly in their appearance, the model will fail. (minimum OpenCV 3.0.0)
            '''
            
            ''' Paulo
            
            '''
        elif self.methodName == 'GOTURN':
            self.__tracker = cv2.TrackerGOTURN_create()
            '''
            The only deep learning-based object detector included in OpenCV.
            It requires additional model files to run (will not be covered in this post). 
            My initial experiments showed it was a bit of a pain to use even though it reportedly handles viewing changes well
            (my initial experiments didn’t confirm this though).
            I’ll try to cover it in a future post, but in the meantime, take a look at Satya’s writeup. (minimum OpenCV 3.2.0)
            '''
            
            ''' Paulo
            Devido a necessidade de um modelo nunca foi testado.
            '''
        elif self.methodName == 'MOSSE':
            self.__tracker = cv2.TrackerMOSSE_create()
            ''' PyImgSearch
            Very, very fast. Not as accurate as CSRT or KCF but a good choice if you need pure speed. (minimum OpenCV 3.4.1)
            '''
            
            ''' Paulo
            Realmente muito rápido e extremamente estável na detecção,
            mas não responde a mudança no tamanho dos objetos que está rastreando.
            Ou seja, não seriamos capazes de verificar a aproximação do objeto.
            '''
        elif self.methodName == 'CSRT':
            self.__tracker = cv2.TrackerCSRT_create()
            '''
            Discriminative Correlation Filter (with Channel and Spatial Reliability). 
            Tends to be more accurate than KCF but slightly slower. (minimum OpenCV 3.4.2)
            '''
        else:
            print("Algortimo '" + self.methodName + "' não reconhecido")
            self.methodName = ""
            
        self.usingTracker = not(self.__tracker is None)
