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
    SetDirection,
    SetMaxVelocity,
    SetDirectionPID,
    SetSpeedPID,
    // ...
  };

  RPiMessageHandler(ControlVariables& variables, Stream& stream, int majorVersion, int minorVersion);
  ~RPiMessageHandler();
  
  void handleMessage(RPiMessageCodes code, int value);
  void processData();
  
private:
  FirmataClass firmata_;
  ControlVariables controlVariables_;
};

extern RPiMessageHandler raspberryPiMessageHandler;

static void raspberryPiIntMessage(byte messageCode, int value)
{
  raspberryPiMessageHandler.handleMessage((RPiMessageHandler::RPiMessageCodes)messageCode, value);
}
#endif //RPI_MESSAGE_HANDLER_H
