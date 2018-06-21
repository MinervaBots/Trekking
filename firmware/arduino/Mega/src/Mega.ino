
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
  state = &search;
  previousState = &search;
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
  rPiCmdMessenger.attach(MessageCodesRPi::kStopEvent, onStopEvent);
  rPiCmdMessenger.attach(MessageCodesRPi::kTargetLost, onRecvTargetLost);
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
    Serial.println("Parar");
    buttonPressStart = 0;
    changeState(reset);
    return;
  }
  if(state == idle)
  {
    Serial.println("Resumir");
    changeState(previousState);
    cameraPid.SetMode(0);
    steeringPid.SetMode(0);
    speedPid.SetMode(0);
  }
  else
  {
    Serial.println("Pausar");
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
  if(actuatorsWrite & ExecutionFlags::kSpeed)
  {
    linearSpeed = constrain(linearSpeed * linearSpeedLock, ESC_MAX_BACKWARD, ESC_MAX_FORWARD);
    esc.write(map(linearSpeed, -1, 1, 0, 180));
  }
  if(actuatorsWrite & ExecutionFlags::kCamera)
  {
    //Serial.print("Camera Servo: \t");
    //Serial.println(cameraServoPosition);
    cameraServo.write(cameraServoPosition);
  }
  if(actuatorsWrite & ExecutionFlags::kSteering)
  {
    steeringServo.write(steeringServoPosition);
  }
}