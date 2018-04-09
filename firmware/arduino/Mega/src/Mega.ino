
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
void buttonISR();

unsigned long lastRun;
volatile unsigned long buttonPressStart;
volatile char buttonNextAction; // -1 (stop), 0 (nothing), 1 (pause)
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

  targets.add(Vector2(40, 20));
  targets.add(Vector2(30, 2));
  targets.add(Vector2(6, 18));
  currentTarget = &targets.get(0);

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
    state(millis() - lastRun);
  }

  cameraPid.Compute();
  steeringPid.Compute();
  speedPid.Compute();

  cameraServo.write(cameraServoPosition);
  steeringServo.write(steeringServoPosition);

  linearSpeed = constrain(linearSpeed, ESC_MAX_BACKWARD, ESC_MAX_FORWARD);
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
    changeState(previusState);
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