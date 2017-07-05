#ifndef Longuinho_cpp
#define Longuinho_cpp

//#include "Pins.h"
#include "Includes.h"

Trekking trekking;
LonguinhoSensoring sensoring;
LonguinhoMotorController motorController;
PIDController pidController;
SimpleMovingAverageFilter<5> filter;
bool emergencyButton, initButton, pauseSwitch;

void setup()
{
  // Inicializa a comunicação Serial e define como saída do logger
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(57600);
  Wire.begin();
  //logger.setPrinter(Serial);

  motorController.setWheelsRadius(0.075);
  motorController.setWheelsDistanceFromRotationAxis(0.145);
  motorController.stop();

  trekking.setSensoring(&sensoring);
  // Define a posição inicial
  sensoring.setPosition(0, 0, 0);
  sensoring.initializeEncoder(&motorController);
  //sensoring.setMagnetometerFilter(&filter);
  sensoring.initializeMPU(20, 10, 10, 40);

  // Adiciona os objetivos
  trekking.addTarget(4, 0);
  trekking.addTarget(4, 2);
  trekking.addTarget(0, 0);

  // Define o ponteiro para a classe de controle dos motores
  trekking.setMotorController(&motorController, 0.2);

  trekking.setBuzzerPin(BUZZER_PIN);

  // [TODO]
  pidController.setTunings(2.5, 0.004, 10);
  pidController.setSetPoint(0);
  pidController.setOutputLimits(-1, 1);
  pidController.setControllerDirection(SystemControllerDirection::Inverse);
  trekking.setSystemController(&pidController);

  trekking.setMaxTimeInRefinedSearch(10000); // 10s
  trekking.setup();
}

//Takes the buttons states
void readButtons()
{
	initButton = digitalRead(INIT_BUTTON_PIN);
  emergencyButton = digitalRead(EMERGENCY_BUTTON_PIN);
  pauseSwitch = digitalRead(PAUSE_SWITCH_PIN);
}

void loop()
{
  readButtons();
  if(pauseSwitch)
  {
    trekking.pause();
  }
  else
  {
    trekking.resume();
  }

  if(initButton && !trekking.isRunning())
  {
    trekking.start();
  }
  if(emergencyButton)
  {
    trekking.emergency();
  }
  trekking.update();
  //float tempo = motorController.movingTime(0,0,10,0,0.5);
}

#endif //Longuinho_cpp
