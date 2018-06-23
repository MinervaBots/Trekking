#include <Servo.h>
#include <PID_v1.h>
#include <math.h>

#include "Pins.h"
#include "Target.h"
#include "Variables.h"
#include "Constants.h"
#include "States.h"
#include "CommandHandlers.h"
#include "RCReceiver.h"
#include "InterruptServiceRoutines.h"
#include "Utils.h"

PID cameraPid(&targetDirection, &cameraServoPosition, &setPointZero, cameraServoKp, cameraServoKi, cameraServoKd, P_ON_M, DIRECT);
PID steeringPid(&targetDirection, &steeringServoPosition, &setPointZero, steeringServoKp, steeringServoKi, steeringServoKd, P_ON_M, DIRECT);
PID speedPid(&targetDistance, &linearSpeed, &setPointZero, speedKp, speedKi, speedKd, P_ON_M, REVERSE);

unsigned long lastRun;
volatile unsigned long buttonPressStart;
SonicArray sonicArray(PIN_ULTRASSONIC_TRIGGER);

void setup()
{ 
  attachHandlers();
  attachRCInterrupts();

  Serial.begin(RPI_BAUD_RATE);
  Serial3.begin(MPU_BAUD_RATE);
  sonicArray.setupInterrupts();
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  cameraServo.attach(CAMERA_SERVO_PIN);
  steeringServo.attach(STEERING_SERVO_PIN);
  esc.attach(ESC_PIN);
  esc.write(ESC_ZERO);

  cameraPid.SetOutputLimits(-1, 1);
  steeringPid.SetOutputLimits(-1, 1);
  speedPid.SetOutputLimits(0, 1);
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
  isRunning = true;
  lastRun = 0;
}

void loop()
{
  if(millis() - lastSignalTime >= 1000)
  {
    esc.write(ESC_ZERO);
    return;
  }

  if (gearPulseWidth > 1500)
  {
    //Serial.println(gearPulseWidth);
    // Manual
    cameraPid.SetMode(MANUAL);
    steeringPid.SetMode(MANUAL);
    speedPid.SetMode(MANUAL);

    esc.writeMicroseconds(elevPulseWidth);
    steeringServo.writeMicroseconds(ruddPulseWidth);
    return;
  }
  else
  {
    // Automático
    cameraPid.SetMode(AUTOMATIC);
    steeringPid.SetMode(AUTOMATIC);
    speedPid.SetMode(AUTOMATIC);
  }
  rPiCmdMessenger.feedinSerialData();
  //mpuCmdMessenger.feedinSerialData();

  handleButton();

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

void handleButton()
{
  if (digitalRead(BUTTON_PIN) == LOW && buttonPressStart == 0) // Pressionado
  {
    delay(200);
    buttonPressStart = millis();
    return;
  }
  if (buttonPressStart == 0)
    return;

  while (digitalRead(BUTTON_PIN) == LOW)
  {
  }

  if (millis() - buttonPressStart > BUTTON_STOP_TIME)
  {
    buttonPressStart = 0;
    changeState(reset);
    return;
  }
  if (state == idle)
  {
    changeState(previousState);
    cameraPid.SetMode(0);
    steeringPid.SetMode(0);
    speedPid.SetMode(0);
  }
  else
  {
    changeState(idle);
    cameraPid.SetMode(1);
    steeringPid.SetMode(1);
    speedPid.SetMode(1);
  }
  buttonPressStart = 0;
}

void pidCompute()
{
  if (computePid & ExecutionFlags::kSpeed)
  {
    speedPid.Compute();
  }
  if (computePid & ExecutionFlags::kCamera)
  {
    cameraPid.Compute();
  }
  if (computePid & ExecutionFlags::kSteering)
  {
    steeringPid.Compute();
  }
}

void writeInActuators()
{
  int value;
  if (actuatorsWrite & ExecutionFlags::kSpeed)
  {
    value = round(mapf(linearSpeed, 0, 1, ESC_ZERO, ESC_MAX_FORWARD));
    esc.write(value);
  }
  if (actuatorsWrite & ExecutionFlags::kCamera)
  {
    value = round(mapf(cameraServoPosition, -1, 1, 0, CAMERA_SERVO_LIMIT));
    cameraServo.write(value);
  }
  if (actuatorsWrite & ExecutionFlags::kSteering)
  {
    value = round(mapf(steeringServoPosition, -1, 1, STEERING_SERVO_MIN_LIMIT, STEERING_SERVO_MAX_LIMIT));
    steeringServo.write(value);
  }
}
