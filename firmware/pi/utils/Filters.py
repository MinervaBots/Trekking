class SimpleLowPassFilter:
    def __init__(self, alpha = 0.5):
        self.alpha = alpha
        self.output = 0
        
    def calculate(self, input : float):
        self.output += self.alpha * (input - self.output)
        
        return self.output
        
class RunningAverageFilter:
    def __init__(self, samplesCount = 16):
        self.samplesCount = samplesCount
        self.__nextSampleId = 0
        self.samplesList = samplesCount * [0]
    
    def calculate(self, input : float):
        self.samplesList[self.__nextSampleId] = input
        self.__nextSampleId += 1
        
        if(self.__nextSampleId >= self.samplesCount):
            self.__nextSampleId = 0
        
        output = 0
        for sample in self.samplesList:
            output += sample
        
        output /= self.samplesCount
        return output
        
class ExponentialFilter:
    def __init__(self, alpha : input = 0.5):
        self.alpha = alpha
        self.output = 0
    
    def calculate(self, input : float):
        self.output += (input - self.output) * self.alpha
        return self.output

class ResponsiveExponentialFilter:
    def __init__(self, sleepEnable, snapMultiplier = 0.01, activityThreshold = 0.2, edge = (-1, 1)):
        self.sleepEnable = sleepEnable
        self.output = 0
        
        self.enableEdgeSnap()
        self.setSnapMultiplier(snapMultiplier)
        self.setActivityThreshold(activityThreshold)
        self.setEdge(*edge)
        
        #    Variaveis usadas no calculo do filtro
        self.sleeping = False
        self.errorEMA = 0
        self.prevOutput = 0
        self.lastActivityTime = 0
        self.output = 0
        
    def setEdge(self, minimum, maximum):
        self.edgeMinimum = minimum
        self.edgeMaximum = maximum
        
    def setSnapMultiplier(self, newMultiplier):
        if (newMultiplier > 1.0):
            newMultiplier = 1.0
        if (newMultiplier < 0.0):
            newMultiplier = 0.0
        self.snapMultiplier = newMultiplier
        
    def setActivityThreshold(self, newThreshold):
        self.activityThreshold = newThreshold
        
    def enableEdgeSnap(self):
        self.edgeSnapEnable = True
        
    def disableEdgeSnap(self):
        self.edgeSnapEnable = False
        
    def calculate(self, input : float):
        self.prevOutput = self.output
        
        # if sleep and edge snap are enabled and the new value is very close to an edge, drag it a little closer to the edges
        # This'll make it easier to pull the output values right to the extremes without sleeping,
        # and it'll make movements right near the edge appear larger, making it easier to wake up
        if (self.sleepEnable and self.edgeSnapEnable):
            if(input < self.activityThreshold):
                input = (input * 2) - self.activityThreshold
            elif(input > self.edgeMaximum - self.activityThreshold):
                input = (input * 2) - self.edgeMaximum + self.activityThreshold
    
        # get difference between new input value and current smooth value
        diff = abs(input - self.output)

        self.errorEMA += ((input - self.output) - self.errorEMA) * 0.4

        # if sleep has been enabled, sleep when the amount of error is below the activity threshold
        if(self.sleepEnable):
            self.sleeping = abs(self.errorEMA) < self.activityThreshold
        

        # if we're allowed to sleep, and we're sleeping
        # then don't update responsiveValue this loop
        # just output the existing responsiveValue
        if(self.sleepEnable and self.sleeping):
            return self.output
        

        # use a 'snap curve' function, where we pass in the diff (x) and get back a number from 0-1.
        # We want small values of x to result in an output close to zero, so when the smooth value is close to the input value
        # it'll smooth out noise aggressively by responding slowly to sudden changes.
        # We want a small increase in x to result in a much higher output value, so medium and large movements are snappy and responsive,
        # and aren't made sluggish by unnecessarily filtering out noise. A hyperbola (f(x) = 1/x) curve is used.
        # First x has an offset of 1 applied, so x = 0 now results in a value of 1 from the hyperbola function.
        # High values of x tend toward 0, but we want an output that begins at 0 and tends toward 1, so 1-y flips this up the right way.
        # Finally the result is multiplied by 2 and capped at a maximum of one, which means that at a certain point all larger movements are maximally snappy

        # then multiply the input by SNAP_MULTIPLER so input values fit the snap curve better.
        snap = self.snapCurve(diff * self.snapMultiplier)

        # when sleep is enabled, the emphasis is stopping on a responsiveValue quickly, and it's less about easing into position.
        # If sleep is enabled, add a small amount to snap so it'll tend to snap into a more accurate position before sleeping starts.
        if(self.sleepEnable):
            snap *= 0.5 + 0.5
        
        # calculate the exponential moving average based on the snap
        self.output += (input - self.output) * snap

        # ensure output is in bounds
        if(self.output < self.edgeMinimum):
            self.output = self.edgeMinimum
        elif(self.output > self.edgeMaximum):
            self.output = self.edgeMaximum
        
        self.outputHasChanged = self.prevOutput != self.output
        return self.output
    
    def snapCurve(self, x : float):
        y = 1.0 / (x + 1.0)
        y = (1.0 - y) * 2.0;
        if(y > 1.0):
            return 1.0
        return y