#include "RPiMessageHandler.h"
#include "TypeUtils.h"

RPiMessageHandler::RPiMessageHandler(ControlVariables& variables, Stream& stream, int majorVersion, int minorVersion) : 
  controlVariables_(variables)
{
  firmata_.setFirmwareVersion(majorVersion, minorVersion);
  firmata_.attach(ANALOG_MESSAGE, raspberryPiIntMessage);
  firmata_.begin(stream);
}

RPiMessageHandler::~RPiMessageHandler()
{
  
}

void RPiMessageHandler::processData()
{
  while(firmata_.available())
  {
    firmata_.processInput();
  }
}

void RPiMessageHandler::handleMessage(RPiMessageCodes code, int value)
{
  switch(code)
  {
    case RPiMessageCodes::SetDirection:
      float dir = mapf(value, -2^15, (2^15) - 1, -1.0, 1.0);
      controlVariables_.setDirection(dir);
      break;
  }
}
