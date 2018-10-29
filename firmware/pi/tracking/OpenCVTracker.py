import cv2
from .Tracker import Tracker
 
class OpenCVTracker(Tracker):
    def __init__(self, detector, detectionIntervalMs, trackingMethodName = None):
        Tracker.__init__(self, detector, trackingMethodName, detectionIntervalMs)

    def setTrackingMethod(self, trackingMethodName):
        self.methodName = trackingMethodName

    def init(self, frame):
        if not self.isRunning:
            return False, [], []
        
        detected, boundingBoxes, directions = self._detect(frame)
        if detected:
            self.clear()
            for box in boundingBoxes:
                self.__multiTracker.add(self.__createTracker(), frame, tuple(box))
                
        return detected, boundingBoxes, directions
    
    def _track(self, frame):
        success, boundingBoxex = self.__multiTracker.update(frame)
        directions = [self.detector.rectToDirection(box) for box in boundingBoxex]
        return success, boundingBoxex, directions
    
    def clear(self):
        self.__multiTracker = cv2.MultiTracker_create()
        
    def __createTracker(self):
        tracker = None
        if self.methodName == '' or self.methodName == 'CASCADE' or self.methodName == None:
            pass
        elif self.methodName == 'BOOSTING':
            tracker = cv2.TrackerBoosting_create()
            ''' PyImgSearch
            Based on the same algorithm used to power the machine learning behind Haar cascades (AdaBoost),
            but like Haar cascades, is over a decade old. This tracker is slow and doesn’t work very well.
            Interesting only for legacy reasons and comparing other algorithms. (minimum OpenCV 3.0.0)
            '''
            
            ''' Paulo
            Extremamente lento
            '''
        elif self.methodName == 'MIL':
            tracker = cv2.TrackerMIL_create()
            ''' PyImgSearch
            Better accuracy than BOOSTING tracker but does a poor job of reporting failure. (minimum OpenCV 3.0.0)
            '''
        elif self.methodName == 'KCF':
            tracker = cv2.TrackerKCF_create()
            ''' PyImgSearch
            Kernelized Correlation Filters. Faster than BOOSTING and MIL.
            Similar to MIL and KCF, does not handle full occlusion well. (minimum OpenCV 3.1.0)
            '''
        elif self.methodName == 'TLD':
            tracker = cv2.TrackerTLD_create()
            ''' PyImgSearch
            I’m not sure if there is a problem with the OpenCV implementation of the TLD tracker or the actual algorithm itself,
            but the TLD tracker was incredibly prone to false-positives. I do not recommend using this OpenCV object tracker. (minimum OpenCV 3.0.0)
            '''
        elif self.methodName == 'MEDIANFLOW':
            tracker = cv2.TrackerMedianFlow_create()
            ''' PyImgSearch
            Does a nice job reporting failures; however, if there is too large of a jump in motion,
            such as fast moving objects, or objects that change quickly in their appearance, the model will fail. (minimum OpenCV 3.0.0)
            '''
            
            ''' Paulo
            É bem rápido, responde a mudanças de escala, reporta oclusão.
            A principio é o melhor algoritmo que temos.
            '''
        elif self.methodName == 'GOTURN':
            tracker = cv2.TrackerGOTURN_create()
            ''' PyImgSearch
            The only deep learning-based object detector included in OpenCV.
            It requires additional model files to run (will not be covered in this post). 
            My initial experiments showed it was a bit of a pain to use even though it reportedly handles viewing changes well
            (my initial experiments didn’t confirm this though).
            I’ll try to cover it in a future post, but in the meantime, take a look at Satya’s writeup. (minimum OpenCV 3.2.0)
            '''
            
            ''' Paulo
            Testado com o reconhecimento de face.
            Não apresentou um funcionamento muito bom, devido a constante reinicialização do rastreador.
            Também não reportou corretamente oclusão do objeto.
            Performance bem fraca.
            
            Seria interessante testar com o cone.
            Caso desejem fazer isso baixem os arquivos nesse link: https://www.dropbox.com/sh/77frbrkmf9ojfm6/AACgY7-wSfj-LIyYcOgUSZ0Ua?dl=0
            e extraiam eles no diretório do main.py
            '''
        elif self.methodName == 'MOSSE':
            tracker = cv2.TrackerMOSSE_create()
            ''' PyImgSearch
            Very, very fast. Not as accurate as CSRT or KCF but a good choice if you need pure speed. (minimum OpenCV 3.4.1)
            '''
            
            ''' Paulo
            Realmente muito rápido e extremamente estável na detecção,
            mas não responde a mudança no tamanho dos objetos que está rastreando.
            Ou seja, não seriamos capazes de verificar a aproximação do objeto.
            '''
        elif self.methodName == 'CSRT':
            tracker = cv2.TrackerCSRT_create()
            ''' PyImgSearch
            Discriminative Correlation Filter (with Channel and Spatial Reliability). 
            Tends to be more accurate than KCF but slightly slower. (minimum OpenCV 3.4.2)
            '''
            
            ''' Paulo:
            Responde muito bem a escola, é relativamente estável, tem uma precisão muito boa.
            Performance ruim para o Pi, e não reporta oclusão.
            '''
        else:
            print("Algortimo '" + self.methodName + "' não reconhecido")
            self.methodName = ""
        
        return tracker
