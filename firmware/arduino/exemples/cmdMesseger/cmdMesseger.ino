#include "CmdMessenger.h"

enum
{
    objDetected,
    servoDirection,
    error,
};

const int BAUD_RATE = 9600;
CmdMessenger c = CmdMessenger(Serial,',',';','/');

void onObjDetected(void)
{
  float value1 = c.readBinArg<float>();
  int value2 = c.readBinArg<int>();
  int value3 = c.readBinArg<int>();
  int value4 = c.readBinArg<int>();
  int value5 = c.readBinArg<int>();

  c.sendCmdStart(servoDirection);
  c.sendCmdBinArg(45);
  c.sendCmdEnd();
}

void onUnknownCommand(void)
{
  c.sendCmd(error, "Comando sem handler definido");
}

void attachHandlers(void)
{
  c.attach(objDetected, onObjDetected);
  c.attach(onUnknownCommand);
}

void setup()
{
  Serial.begin(BAUD_RATE);
  attachHandlers();    
}

void loop()
{
  c.feedinSerialData();
}
