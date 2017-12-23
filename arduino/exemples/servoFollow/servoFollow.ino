#include <PID_v1.h>

#include <Servo.h>

#include <CmdMessenger.h>

enum
{
    targetData,
    servoDirection,
    error,
};

const int BAUD_RATE = 9600;
CmdMessenger c = CmdMessenger(Serial,',',';','/');

double objectDirection = 0;
double servoPosition = 0;
double setPoint = 0;
double Kp=1.5, Ki=0.3, Kd=1.2;
PID pid(&objectDirection, &servoPosition, &setPoint, Kp, Ki, Kd, P_ON_M, REVERSE);

Servo servo;


void onObjDetected(CmdMessenger* cmdMessenger)
{
  objectDirection = cmdMessenger->readBinArg<double>();
  
  int value2 = cmdMessenger->readBinArg<int>();
  int value3 = cmdMessenger->readBinArg<int>();
  int value4 = cmdMessenger->readBinArg<int>();
  int value5 = cmdMessenger->readBinArg<int>();

  cmdMessenger->sendCmdStart(servoDirection);
  c.sendCmdBinArg<double>(servoPosition);
  c.sendCmdEnd();
}

void onUnknownCommand(CmdMessenger* cmdMessenger)
{
  cmdMessenger->sendCmdStart(error);
  cmdMessenger->sendCmdArg("Comando sem handler definido");
  cmdMessenger->sendCmdEnd();
}

void attachHandlers()
{
  c.attach(targetData, onObjDetected);
  c.attach(onUnknownCommand);
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(BAUD_RATE);
  attachHandlers();
  
  pid.SetOutputLimits(0, 255);
  pid.SetMode(AUTOMATIC);
  pid.SetSampleTime(0);
  
  servo.attach(9);
}

bool state = false;
void loop()
{
  c.feedinSerialData();
  pid.Compute();
  servo.write(servoPosition);
}
