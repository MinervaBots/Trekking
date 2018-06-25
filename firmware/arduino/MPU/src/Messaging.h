#ifndef MESSAGING_H
#define MESSAGING_H

#include <Arduino.h>
#include <CmdMessenger.h>

enum MessageCodes
{
    kLog,
    kUpdateTransform,
    kSetPosition,
};

enum LogMessageSubCodes
{
    kInfo,
    kWarning,
    kError
};

void attachCallbacks(CmdMessenger *messenger);
void setPosition(CmdMessenger *messenger);

#endif //MESSAGING_H