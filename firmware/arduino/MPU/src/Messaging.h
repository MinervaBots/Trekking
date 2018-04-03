#ifndef MESSAGING_H
#define MESSAGING_H

#include <Arduino.h>

enum MessageCodes
{
  Log,
  UpdateTransform
};

enum LogMessageSubCodes
{
  Info,
  Warning,
  Error
};

void writeCmdStart(int cmdId);
void writeCmdArg(int arg);
void writeCmdArg(const String &s);
void writeCmdEnd();

#endif //MESSAGING_H