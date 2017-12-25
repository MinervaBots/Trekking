

def info(msg, recvTime):
    print ("INFO: {}".format(msg))

def error(msg, recvTime):
    print ("ERROR: {}".format(msg))

def mpuData(accX, accY, heading, recvTime):
    print("Aceleração X: {} \t Y:{} \t Heading: {}".format(accX, accY, heading))
