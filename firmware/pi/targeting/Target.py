from random import randint


class Target:
    def __init__(self, rect, distance, direction, color = None):
        self.rect = rect
        self.distance = distance
        self.direction = direction
        self.color = color
        if self.color is None:
            self.color = (randint(0, 255), randint(0, 255), randint(0, 255))
    
    
    def update(self, rect, distance, direction, color = None):
        self.rect = rect
        self.distance = distance
        self.direction = direction
        if color is not None:
            self.color = color