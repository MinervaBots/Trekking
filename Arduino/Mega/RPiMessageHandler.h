#ifndef RPI_MESSAGE_HANDLER_H
#define RPI_MESSAGE_HANDLER_H

#include <Firmata.h>
#include "ControlVariables.h"

using namespace firmata;


class RPiMessageHandler
{
public:
  enum RPiMessageCodes
  {
    SetTargetData,
    SetMaxVelocity,
    SetDirectionPID,
    SetSpeedPID,
    // ...
  };

  RPiMessageHandler(ControlVariables& variables, Stream& stream, int majorVersion, int minorVersion);
  ~RPiMessageHandler();
  
  void handleMessage(RPiMessageCodes code, byte *arrayPointer, byte byteCount);
  void processData();
  
private:
  FirmataClass firmata_;
  ControlVariables controlVariables_;
};

extern RPiMessageHandler raspberryPiMessageHandler;

static void raspberryPiGenericCallbackFunction(byte messageCode, int value)
{
  raspberryPiMessageHandler.handleMessage((RPiMessageHandler::RPiMessageCodes)messageCode, (byte*)value, sizeof(int));
}

static void raspberryPiSysexCallbackFunction(byte messageCode, byte byteCount, byte *arrayPointer)
{
  raspberryPiMessageHandler.handleMessage((RPiMessageHandler::RPiMessageCodes)messageCode, arrayPointer, byteCount);
}
#endif //RPI_MESSAGE_HANDLER_H
