#include <Servo.h>
#include <PID_v1.h>
#include <math.h>

#include "TypeUtils.h"
#include "Pins.h"
#include "Target.h"
#include "Variables.h"
#include "Constants.h"
#include "States.h"
#include "CommandHandlers.h"

PID cameraPid(&targetDirection, &cameraServoPosition, &setPointZero, cameraServoKp, cameraServoKi, cameraServoKd, P_ON_M, DIRECT);
PID steeringPid(&targetDirection, &steeringServoPosition, &setPointZero, steeringServoKp, steeringServoKi, steeringServoKd, P_ON_M, DIRECT);
PID speedPid(&targetDistance, &linearSpeed, &setPointZero, speedKp, speedKi, speedKd, P_ON_M, REVERSE);

void attachHandlers();
void buttonISR();

unsigned long lastRun;
volatile unsigned long buttonPressStart;
SonicArray sonicArray(PIN_ULTRASSONIC_TRIGGER);

void setup()
{
  attachHandlers();
  Serial.begin(RPI_BAUD_RATE);
  Serial3.begin(MPU_BAUD_RATE);
  sonicArray.setupChangeInterrupt();
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  cameraServo.attach(CAMERA_SERVO_PIN);
  steeringServo.attach(STEERING_SERVO_PIN);
  esc.attach(ESC_PIN);
  esc.write(ESC_ZERO);

  cameraPid.SetOutputLimits(-1, 1);
  steeringPid.SetOutputLimits(-1, 1);
  speedPid.SetOutputLimits(-1, 1);
  cameraPid.SetMode(AUTOMATIC);
  steeringPid.SetMode(AUTOMATIC);
  speedPid.SetMode(AUTOMATIC);

  targets.add(Target(40, 20, true));
  targets.add(Target(30, 2, true));
  targets.add(Target(6, 20, false));
  targets.add(Target(6, 18, true));
  currentTarget = targets.get(0);

  state = &idle;
  previousState = &idle;
  lastRun = 0;
  isRunning = true;
}

void loop()
{
  handleButton();
  rPiCmdMessenger.feedinSerialData();
  mpuCmdMessenger.feedinSerialData();

  if (isRunning)
  {
    computePid = ExecutionFlags::kAll;
    actuatorsWrite = ExecutionFlags::kAll;
    (*state)(millis() - lastRun);
  }

  pidCompute();
  writeInActuators();
  lastRun = millis();
}

void attachHandlers()
{
  rPiCmdMessenger.attach(MessageCodesRPi::kTargetFound, onRecvTargetFound);
  rPiCmdMessenger.attach(MessageCodesRPi::kTargetLost, onRecvTargetLost);
  rPiCmdMessenger.attach(MessageCodesRPi::kStopEvent, onStopEvent);
  rPiCmdMessenger.attach(onRecvUnknownCommand);

  mpuCmdMessenger.attach(MessageCodesMPU::kMpuData, onRecvMpuData);
  mpuCmdMessenger.attach(MessageCodesMPU::kMpuLog, onRecvMpuLog);
  mpuCmdMessenger.attach(onRecvUnknownCommand);
}

void handleButton()
{
  if (digitalRead(BUTTON_PIN) == LOW && buttonPressStart == 0) // Pressionado
  {
    delay(200);
    buttonPressStart = millis();
    return;
  }
  if(buttonPressStart == 0) return;

  while(digitalRead(BUTTON_PIN) == LOW) {  }

  if (millis() - buttonPressStart > BUTTON_STOP_TIME)
  {
    //Serial.println("Parar");
    buttonPressStart = 0;
    changeState(reset);
    return;
  }
  if(state == idle)
  {
    //Serial.println("Resumir");
    changeState(previousState);
    cameraPid.SetMode(0);
    steeringPid.SetMode(0);
    speedPid.SetMode(0);
  }
  else
  {
    //Serial.println("Pausar");
    changeState(idle);
    cameraPid.SetMode(1);
    steeringPid.SetMode(1);
    speedPid.SetMode(1);
  }
  buttonPressStart = 0;
}

ISR(PCINT0_vect)
{
  sonicArray.handleEcho(SonicArray::Vector::VECTOR_0);
}

ISR(PCINT2_vect)
{
  sonicArray.handleEcho(SonicArray::Vector::VECTOR_2);
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
  int value;
  delay(50);

  rPiCmdMessenger.sendCmdStart(MessageCodesRPi::kRPiLog);
  rPiCmdMessenger.sendCmdArg("writeInActuators");

  if(actuatorsWrite & ExecutionFlags::kSpeed)
  {
    value = round(mapf(linearSpeed, -1, 1, ESC_MAX_BACKWARD, ESC_MAX_FORWARD));
    esc.write(value);
    rPiCmdMessenger.sendCmdArg(linearSpeed);
    rPiCmdMessenger.sendCmdArg(value);
    //rPiCmdMessenger.sendCmdArg(value);
  }
  if(actuatorsWrite & ExecutionFlags::kCamera)
  {
    value = round(mapf(cameraServoPosition, -1, 1, 0, CAMERA_SERVO_LIMIT));
    cameraServo.write(value);
    //rPiCmdMessenger.sendCmdArg(cameraServoPosition);
  }
  if(actuatorsWrite & ExecutionFlags::kSteering)
  {
    value = round(mapf(steeringServoPosition, -1, 1, STEERING_SERVO_MIN_LIMIT, STEERING_SERVO_MAX_LIMIT));
    steeringServo.write(value);
    //rPiCmdMessenger.sendCmdArg(steeringServoPosition);
  }
  rPiCmdMessenger.sendCmdEnd();
  delay(15);
}