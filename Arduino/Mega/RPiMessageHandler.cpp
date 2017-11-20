#include "RPiMessageHandler.h"
#include "TypeUtils.h"

RPiMessageHandler::RPiMessageHandler(ControlVariables& variables, Stream& stream, int majorVersion, int minorVersion) : 
  controlVariables_(variables)
{
  firmata_.setFirmwareVersion(majorVersion, minorVersion);
  firmata_.attach(ANALOG_MESSAGE, raspberryPiGenericCallbackFunction);
  firmata_.attach(START_SYSEX, raspberryPiSysexCallbackFunction);
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

void RPiMessageHandler::handleMessage(RPiMessageCodes code, byte *arrayPointer, byte byteCount)
{
  switch(code)
  {
    case RPiMessageCodes::SetTargetData:
      auto targetData = (TargetPosition*)arrayPointer;
      // Determinar direção, e distancia pro alvo
      // Setar isso em controlVariables_
      break;
  }
}
