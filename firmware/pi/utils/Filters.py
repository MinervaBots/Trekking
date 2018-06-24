class RunningAverageFilter:
  def __init__(self, samplesCount = 16):
    self.samplesCount = 16
    self.__nextSampleId = 0
    self.samplesList = 16 * [0]
  
  def calculate(input : float):
    self.samplesList[self.__nextSampleId++] = input
    if(self.__nextSampleId >= samplesCount):
      self.__nextSampleId = 0
    
    output = 0
    for sample in self.samplesList:
      output += sample
    
    output /= samplesCount
    return output
    
class ExponentialFilter:
  def __init__(self, alpha : input):
    self.alpha = alpha
    self.output = 0
  
  def calculate(input : float):
    self.output += (input - self.output) * self.alpha
    return self.output
