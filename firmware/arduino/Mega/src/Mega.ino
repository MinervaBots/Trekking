
#include <Servo.h>
#include <PID_v1.h>

#include "Pins.h"
#include "Target.h"
#include "Variables.h"
#include "Constants.h"
#include "States.h"
#include "CommandHandlers.h"

PID cameraPid(&targetDirection, &cameraServoPosition, &setPointZero, cameraServoKp, cameraServoKi, cameraServoKd, P_ON_M, DIRECT);
PID steeringPid(&targetDirection, &steeringServoPosition, &setPointZero, steeringServoKp, steeringServoKi, steeringServoKd, P_ON_M, DIRECT);
PID speedPid(&targetDistance, &linearSpeed, &setPointZero, speedKp, speedKi, speedKd, P_ON_M, DIRECT);

void attachHandlers();

unsigned long lastRun;

SonicArray sonicArray(PIN_ULTRASSONIC_TRIGGER);

void setup()
{
  attachHandlers();
  Serial.begin(RPI_BAUD_RATE);
  Serial1.begin(MPU_BAUD_RATE);
  sonicArray.setupChangeInterrupt();

  cameraServo.attach(CAMERA_SERVO_PIN);
  steeringServo.attach(STEERING_SERVO_PIN);
  esc.attach(ESC_PIN);

  cameraPid.SetOutputLimits(0, degrees(CAMERA_SERVO_LIMIT));
  steeringPid.SetOutputLimits(0, degrees(STEERING_SERVO_LIMIT));
  speedPid.SetOutputLimits(ESC_MAX_BACKWARD, ESC_MAX_FORWARD);

  targets.add(Target(40, 20, true));
  targets.add(Target(30, 2, true));
  targets.add(Target(6, 20, false));
  targets.add(Target(6, 18, true));
  currentTarget = targets.get(0);

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

  if (isRunning)
  {
    state(millis() - lastRun);
  }

  cameraServo.write(cameraServoPosition);
  steeringServo.write(steeringServoPosition);

  linearSpeed = constrain(linearSpeed * linearSpeedLock, ESC_MAX_BACKWARD, ESC_MAX_FORWARD);
  esc.write(map(linearSpeed, -1, 1, 0, 180));

  lastRun = millis();
}

void attachHandlers()
{
  rPiCmdMessenger.attach(MessageCodesRPi::kTargetFound, onRecvTargetFound);
  rPiCmdMessenger.attach(MessageCodesRPi::kTargetLost, onRecvTargetLost);
  rPiCmdMessenger.attach(onRecvUnknownCommand);

  mpuCmdMessenger.attach(MessageCodesMPU::kMpuData, onRecvMpuData);
  mpuCmdMessenger.attach(MessageCodesMPU::kMpuLog, onRecvMpuLog);
  mpuCmdMessenger.attach(onRecvUnknownCommand);
}

ISR(PCINT0_vect)
{
  sonicArray.handleEcho(SonicArray::Vector::VECTOR_0);
}

ISR(PCINT2_vect)
{
  sonicArray.handleEcho(SonicArray::Vector::VECTOR_2);
}
