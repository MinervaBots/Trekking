
#include <Servo.h>
#include <PID_v1.h>

#include "Pins.h"
#include "Target.h"
#include "Variables.h"
#include "Constants.h"
#include "States.h"
#include "CommandHandlers.h"

PID cameraPid(&cameraDirection, &cameraServoPosition, &setPointZero, cameraServoKp, cameraServoKi, cameraServoKd, P_ON_M, DIRECT);
PID steeringPid(&targetDirection, &steeringServoPosition, &setPointZero, steeringServoKp, steeringServoKi, steeringServoKd, P_ON_M, DIRECT);
PID speedPid(&targetDistance, &linearSpeed, &setPointZero, speedKp, speedKi, speedKd, P_ON_M, DIRECT);

void attachHandlers();
void buttonISR();

unsigned long lastRun;
volatile unsigned long buttonPressStart;
volatile char buttonNextAction; // -1 (stop), 0 (nothing), 1 (pause)
SonicArray sonicArray(PIN_ULTRASSONIC_TRIGGER);

void setup()
{
  attachHandlers();
  Serial.begin(RPI_BAUD_RATE);
  Serial3.begin(MPU_BAUD_RATE);
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

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, CHANGE);

  state = idle;
}

void loop()
{
  proccessButtonPress();
  rPiCmdMessenger.feedinSerialData();
  mpuCmdMessenger.feedinSerialData();

  targetDirection = targetDirectionFiltered.getAverage();
  targetDistance = targetDistanceFiltered.getAverage();

  if (isRunning)
  {
    computePid = ExecutionFlags::kAll;
    actuatorsWrite = ExecutionFlags::kAll;
    state(millis() - lastRun);
  }

  pidCompute();
  writeInActuators();

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

void proccessButtonPress()
{
  if(buttonNextAction == 1)
  {
    changeState(idle);
    cameraPid.SetMode(0);
    steeringPid.SetMode(0);
    speedPid.SetMode(0);
  }
  else if(buttonNextAction == -1)
  {
    changeState(reset);
    cameraPid.Initialize(true);
    steeringPid.Initialize(true);
    speedPid.Initialize(true);
  }
  else
  {
    changeState(previousState);
    cameraPid.SetMode(1);
    steeringPid.SetMode(1);
    speedPid.SetMode(1);
  }
  buttonNextAction = 0;
}

ISR(PCINT0_vect)
{
  sonicArray.handleEcho(SonicArray::Vector::VECTOR_0);
}

ISR(PCINT2_vect)
{
  sonicArray.handleEcho(SonicArray::Vector::VECTOR_2);
}

void buttonISR()
{
  if (digitalRead(BUTTON_PIN) == LOW) // Pressionado
  {
    buttonPressStart = millis();
    return;
  }


  if (millis() - buttonPressStart > BUTTON_STOP_TIME)
  {
    buttonNextAction = -1;
    return;
  }
  // Se já estiver pausado, resume a execução
  buttonNextAction = (buttonNextAction == 1) ? 0 : 1;
}

void pidCompute()
{
  if(computePid & ExecutionFlags::kSpeed)
  {
    speedPid.Compute();
  }
  if(computePid & ExecutionFlags::kCamera)
  {
    cameraPid.Compute();
  }
  if(computePid & ExecutionFlags::kSteering)
  {
    steeringPid.Compute();
  }
}

void writeInActuators()
{
  if(actuatorsWrite & ExecutionFlags::kSpeed)
  {
    linearSpeed = constrain(linearSpeed * linearSpeedLock, ESC_MAX_BACKWARD, ESC_MAX_FORWARD);
    esc.write(map(linearSpeed, -1, 1, 0, 180));
  }
  if(actuatorsWrite & ExecutionFlags::kCamera)
  {
    cameraServo.write(cameraServoPosition);
  }
  if(actuatorsWrite & ExecutionFlags::kSteering)
  {
    steeringServo.write(steeringServoPosition);
  }
}