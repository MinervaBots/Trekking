#include "Messaging.h"
#include "Constants.h"
#include "Variables.h"

void attachCallbacks(CmdMessenger *messenger)
{
  messenger->attach(MessageCodes::kSetPosition, setPosition);
}

void setPosition(CmdMessenger *messenger)
{
  transform.position.set(messenger->readFloatArg(), messenger->readFloatArg(), 0);
  transform.heading = messenger->readFloatArg();
}