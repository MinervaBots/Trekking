#include <Servo.h>
#include <PID_v1.h>

#include "Pins.h"
#include "Variables.h"
#include "Constants.h"
#include "States.h"
#include "CommandHandlers.h"



PID cameraPid(&targetDirection, &cameraServoPosition, &setPointZero, cameraServoKp, cameraServoKi, cameraServoKd, P_ON_M, DIRECT);
PID steeringPid(&targetDirection, &steeringServoPosition, &setPointZero, steeringServoKp, steeringServoKi, steeringServoKd, P_ON_M, DIRECT);
PID speedPid(&targetDistance, &linearSpeed, &setPointZero, speedKp, speedKi, speedKd, P_ON_M, DIRECT);

void attachHandlers();

unsigned long lastRun;


void setup()
{
  Serial.begin(RPI_BAUD_RATE);
  Serial1.begin(MPU_BAUD_RATE);
  attachHandlers();
  
  cameraServo.attach(CAMERA_SERVO_PIN);
  steeringServo.attach(STEERING_SERVO_PIN);
  esc.attach(ESC_PIN);
  
  cameraPid.SetOutputLimits(0, degrees(CAMERA_SERVO_LIMIT));
  steeringPid.SetOutputLimits(0, degrees(STEERING_SERVO_LIMIT));
  speedPid.SetOutputLimits(ESC_PID_LIMIT_MIN, ESC_PID_LIMIT_MAX);

  targets.add(Vector2(40, 20));
  targets.add(Vector2(30, 2));
  targets.add(Vector2(6, 18));
  currentTarget = &targets.get(0);

  // Pinagem de placeholder
  ultrassonicArray.setForward(11, 11, 1);
  ultrassonicArray.setForwardLeft(11, 11, 0.5);
  ultrassonicArray.setForwardRight(11, 11, -0.5);
  ultrassonicArray.setLeft(11, 11, 0.25);
  ultrassonicArray.setRight(11, 11, -0.25);
  
  state = idle;
}

void loop()
{
  rPiCmdMessenger.feedinSerialData();
  mpuCmdMessenger.feedinSerialData();
  
  targetDirection = targetDirectionFiltered.getAverage();
  targetDistance = targetDistanceFiltered.getAverage();

  cameraPid.Compute();
  steeringPid.Compute();
  speedPid.Compute();
  
  if(isRunning)
  {
    state(millis() - lastRun);
  }

  cameraServo.write(cameraServoPosition);
  steeringServo.write(steeringServoPosition);
  esc.write(map(linearSpeed, -1, 1, 0, 180));

  lastRun = millis();
}


void attachHandlers()
{
  rPiCmdMessenger.attach(targetData, onRecvTargetData);
  rPiCmdMessenger.attach(onRecvUnknownCommand);
  
  mpuCmdMessenger.attach(mpuData, onRecvMpuData);
  mpuCmdMessenger.attach(onRecvUnknownCommand);
}
