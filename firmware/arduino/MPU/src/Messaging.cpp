#include "Messaging.h"

void writeCmdStart(int cmdId)
{
  Serial.print(cmdId);
}

void writeCmdArg(int arg)
{
  Serial.print(",");
  Serial.print(arg);
}

void writeCmdArg(const String &s)
{
  Serial.print(",");
  Serial.print(s);
}

void writeCmdEnd()
{
  Serial.print(";");
}
