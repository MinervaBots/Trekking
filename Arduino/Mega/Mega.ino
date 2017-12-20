#include <Servo.h>
#include <PID_v1.h>
#include <CmdMessenger.h>

#include "Pins.h"
#include "Variables.h"
#include "Constants.h"
#include "CommandHandlers.h"

unsigned long lastRun;


Servo cameraServo, steeringServo, esc;
PID cameraPid(&targetDirection, &cameraServoPosition, &setPointZero, cameraServoKp, cameraServoKi, cameraServoKd, DIRECT);
PID steeringPid(&targetDirection, &steeringServoPosition, &setPointZero, steeringServoKp, steeringServoKi, steeringServoKd, DIRECT);
PID speedPid(&targetDistance, &linearSpeed, &setPointZero, speedKp, speedKi, speedKd, DIRECT);

CmdMessenger rPiCmdMessenger = CmdMessenger(Serial,',',';','/');
CmdMessenger mpuCmdMessenger = CmdMessenger(Serial1,',',';','/');
void attachHandlers();

void (*state)(unsigned long);


void setup()
{
  Serial.begin(RPI_BAUD_RATE);
  Serial1.begin(MPU_BAUD_RATE);
  
  cameraServo.attach(CAMERA_SERVO_PIN);
  steeringServo.attach(STEERING_SERVO_PIN);
  esc.attach(ESC_PIN);
  
  cameraPid.setOutputLimits(0, CAMERA_SERVO_LIMIT);
  steeringPid.setOutputLimits(0, STEERING_SERVO_LIMIT);
  speedPid.setOutputLimits(0, ESC_PID_LIMIT);
}

void loop()
{
  if(isRunning)
  {
    state(millis() - lastRun);
  }
  
  cameraServo.write(cameraServoPosition);
  steeringServo.write(steeringServoPosition);
  esc.write(linearSpeed);

  lastRun = millis();
}


void attachHandlers()
{
  rPiCmdMessenger.attach(targetData, onRecvTargetData);
  mpuCmdMessenger.attach(mpuData, onRecvMpuData);
  rPiCmdMessenger.attach(onRecvUnknownCommand);
  mpuCmdMessenger.attach(onRecvUnknownCommand);
}
