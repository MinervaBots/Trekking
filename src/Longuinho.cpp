#ifndef Longuinho_cpp
#define Longuinho_cpp

//#include "Pins.h"
#include "Includes.h"

Trekking trekking;
LonguinhoSensoring sensoring;
LonguinhoMotorController motorController;
PIDController pidController;
Logger *Log = new PrintLogger(Serial, LogLevel::Verboses);

void testMotors()
{
  motorController.setWheelsRadius(0.075);
  motorController.setWheelsDistanceFromRotationAxis(0.150);

  motorController.move(1, 0);
  delay(2000);
  motorController.move(-1, 0);
  delay(2000);
  motorController.move(0, 1);
  delay(2000);
  motorController.move(0, -1);
  delay(2000);
  motorController.move(0, 0);
  delay(2000);
}

void setup()
{
  // Inicializa a comunicação Serial e define como saída do logger
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(57600);
  Wire.begin();

  //logger.setPrinter(Serial);

  motorController.setWheelsRadius(0.075);
  motorController.setWheelsDistanceFromRotationAxis(0.150);

  // Define a posição inicial
  sensoring.intializePosition(0, 0, 0);
  sensoring.initializeEncoder(&motorController);

  sensoring.initializeMPU(20, 10, 10, 40);
  trekking.setSensoring(&sensoring);

  // Adiciona os objetivos
  trekking.addTarget(10, 0);
  trekking.addTarget(30, 2);
  trekking.addTarget(6, 18);

  // Define o ponteiro para a classe de controle dos motores
  trekking.setMotorController(&motorController, 0.2);

  trekking.setBuzzerPin(BUZZER_PIN);

  // [TODO]
  pidController.setTunings(10, 1, 3);
  pidController.setSetPoint(0);
  pidController.setOutputLimits(-1, 1);
  trekking.setSystemController(&pidController);

  trekking.setMaxTimeInRefinedSearch(10000); // 10s
  trekking.setup();
}

bool emergencyButton;
void readButtons()
{
	bool initButton = digitalRead(INIT_BUTTON_PIN);
  emergencyButton = digitalRead(EMERGENCY_BUTTON_PIN);

  if(initButton && !trekking.isRunning())
  {
    trekking.start();
  }
  if(emergencyButton)
  {
    trekking.emergency();
  }

	bool operationModeSwitch = digitalRead(OPERATION_MODE_SWITCH_PIN);
  trekking.setOperationMode(operationModeSwitch);
}

void loop()
{
  readButtons();
  trekking.update();
}

#endif //Longuinho_cpp
