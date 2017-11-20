#include "Locomotion.h"
#include "ControlVariables.h"
#include "RPiMessageHandler.h"
#include "DistanceToCamera.h"
#include "Constants.h"
#include "FiniteStateMachine.h"
#include "IdleState.h"
#include "FollowDirectionState.h"

Locomotion locomotion(11, 10);
ControlVariables controlVariables;
RPiMessageHandler raspberryPiMessageHandler(controlVariables, Serial, FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION);
DistanceToCamera coneDistance(CONE_REAL_HEIGHT, FOCAL_LENGHT, SENSOR_HEIGHT, IMAGE_PIXEL_HEIGHT);
FiniteStateMachine stateMachine;


IdleState idleState(&locomotion);
FollowDirectionState followDirectionState(&locomotion, &controlVariables);

void setup()
{
  Serial.begin(57600);
  stateMachine.addTransition(1, &idleState, &followDirectionState);
}

void loop()
{
  raspberryPiMessageHandler.processData();
}
