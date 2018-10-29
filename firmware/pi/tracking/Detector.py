class Detector:
    def __init__(self, resolution):
        self.resolution = resolution
        
    def find(self, frame):
        raise NotImplementedError()
        
    # Faz uma interpolação para calcular a direção
    # do centro do objeto relativo ao centro da tela
    # Mapeia a posição em pixels na tela para uma direção entre -1 e 1
    def rectToDirection(self, rect):
        objCenterX = (((rect[0] + (rect[2] / 2.0)) / self.resolution[0]) - 0.5) * 2
        return objCenterX

    @staticmethod
    def rectToPoints(rect):
        p1 = int(rect[0]), int(rect[1])
        p2 = int(rect[0] + rect[2]), int(rect[1] + rect[3])
        return p1, p2