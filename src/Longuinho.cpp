#ifndef Longuinho_cpp
#define Longuinho_cpp

#include "Includes.h"

PrintLogger logger;

Trekking trekking;
LonguinhoSensoring sensoring;
LonguinhoMotorController motorController;
PIDController pidController;

void setup()
{
  // Inicializa a comunicação Serial e define como saída do logger
  Serial.begin(9600);
  logger.setPrinter(Serial);


  // Define a posição inicial
  sensoring.intializePosition(3, 3, 0);
  sensoring.initializeEncoder(&motorController);
  sensoring.initializeMPU(20, 10, 10, 40);
  trekking.setSensoring(&sensoring);

  // Adiciona os objetivos
  trekking.addTarget(40, 20);
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

  trekking.setup();
}

void readButtons()
{
	bool initButton = digitalRead(INIT_BUTTON_PIN);
  if(initButton && trekking.isRunning())
  {
    trekking.start();
  }

	bool emergencyButton = digitalRead(EMERGENCY_BUTTON_PIN);
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
