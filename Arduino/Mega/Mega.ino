#include "ControlVariables.h"
#include "RPiMessageHandler.h"
#include "DistanceToCamera.h"
#include "Constants.h"

ControlVariables controlVariables;
RPiMessageHandler raspberryPiMessageHandler(controlVariables, Serial, FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION);
DistanceToCamera coneDistance(CONE_REAL_HEIGHT, FOCAL_LENGHT, SENSOR_HEIGHT, IMAGE_PIXEL_HEIGHT);

void setup()
{
  Serial.begin(57600);
}

void loop()
{
  raspberryPiMessageHandler.processData();
}
