#include "ControlVariables.h"
#include "RPiMessageHandler.h"

ControlVariables controlVariables;
RPiMessageHandler raspberryPiMessageHandler(controlVariables, Serial, FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION);

void setup()
{
  Serial.begin(57600);
}

void loop()
{
  raspberryPiMessageHandler.processData();
}
