# Exemplo em python para uso da biblioteca PyCmdMessager
# A parte que roda no arduino se encontra em ../arduino/exemples/cmdMesseger
import PyCmdMessenger
 
arduino = PyCmdMessenger.ArduinoBoard("COM3", baud_rate = 9600)

commands = [["objDetected", "fiiii"],
            ["servoDirection", "i"],
            ["error", "s"]]

# Initialize the messenger
c = PyCmdMessenger.CmdMessenger(arduino, commands)

c.send("objDetected", 0.1, 100, 20, 30, 50)
# Recebe. Dee retornar ["servoDirection", [45], TIME_RECIEVED]
msg = c.receive()
print(msg)
