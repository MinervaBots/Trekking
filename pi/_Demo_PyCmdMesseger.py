# Exemplo em python para uso da biblioteca PyCmdMessager
# A parte que roda no arduino se encontra em ../arduino/exemples/cmdMesseger
import PyCmdMessenger
import CommandMessenger

arduino = PyCmdMessenger.ArduinoBoard("COM3", baud_rate = 9600)

def error(msg, recvTime):
    print(msg)
    print(recvTime)
    
commands = [["targetData", "fiiii"],
            #["servoDirection", "i"],
            ["error", "s", error]]

# Initialize the messenger
cmdMessenger = CommandMessenger.CommandMessenger(arduino, commands)
cmdMessenger.start()

cmdMessenger.send("targetData", 0.1, 100, 20, 30, 50)
