#include <Servo.h>
#include <PID_v1.h>
#include <CmdMessenger.h>

#include "Pins.h"
#include "Variables.h"
#include "Constants.h"
#include "CommandHandlers.h"


Servo cameraServo, steeringServo, esc;


PID cameraPid(&targetDirection, &cameraServoPosition, &setPointZero, cameraServoKp, cameraServoKi, cameraServoKd, P_ON_M, DIRECT);
PID steeringPid(&targetDirection, &steeringServoPosition, &setPointZero, steeringServoKp, steeringServoKi, steeringServoKd, P_ON_M, DIRECT);
PID speedPid(&targetDistance, &linearSpeed, &setPointZero, speedKp, speedKi, speedKd, P_ON_M, DIRECT);


void attachHandlers();
CmdMessenger rPiCmdMessenger = CmdMessenger(Serial,',',';','/');
CmdMessenger mpuCmdMessenger = CmdMessenger(Serial1,',',';','/');


unsigned long lastRun;
void (*state)(unsigned long);


void setup()
{
  Serial.begin(RPI_BAUD_RATE);
  Serial1.begin(MPU_BAUD_RATE);
  attachHandlers();
  
  cameraServo.attach(CAMERA_SERVO_PIN);
  steeringServo.attach(STEERING_SERVO_PIN);
  esc.attach(ESC_PIN);
  
  cameraPid.SetOutputLimits(0, CAMERA_SERVO_LIMIT);
  steeringPid.SetOutputLimits(0, STEERING_SERVO_LIMIT);
  speedPid.SetOutputLimits(0, ESC_PID_LIMIT);
}

void loop()
{
  rPiCmdMessenger.feedinSerialData();
  mpuCmdMessenger.feedinSerialData();
  
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
