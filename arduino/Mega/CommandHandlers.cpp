#include "CommandHandlers.h"
#include "Variables.h"
#include "Constants.h"

unsigned long currTime; // Tempo atual
unsigned long prevTime = millis();// Tempo da última leitura

float prevAccelerationX, prevAccelerationY;
float prevVelocityX, prevVelocityY;
float velocityX, velocityY;

void onRecvMpuData(CmdMessenger *cmdMesseger)
{
  currTime = millis();
  unsigned long deltaTimeInSeconds = (currTime - prevTime) / 1000; // 1000 adicionado para obter as mesmas unidades
  
  int accelerationX = cmdMesseger->readBinArg<float>();
  int accelerationY = cmdMesseger->readBinArg<float>();
  currentTransform.heading = cmdMesseger->readBinArg<float>();

  velocityX += (1/2)*(accelerationX + prevAccelerationX) * deltaTimeInSeconds;
  prevAccelerationX = accelerationX;
    
  velocityY += (1/2)*(accelerationY + prevAccelerationY) * deltaTimeInSeconds;
  prevAccelerationY = accelerationY;
  
  currentTransform.x += (1/2)*(velocityX + prevVelocityX) * deltaTimeInSeconds;
  prevVelocityX = velocityX;
  
  currentTransform.y += (1/2)*(velocityY + prevVelocityY) * deltaTimeInSeconds;
  prevVelocityY = velocityY;

  prevTime = millis();
}

void onRecvTargetData(CmdMessenger *cmdMesseger)
{
  targetDirection = cmdMesseger->readBinArg<float>();

  int x = cmdMesseger->readBinArg<int>();
  int y = cmdMesseger->readBinArg<int>();
  int w = cmdMesseger->readBinArg<int>();
  int h = cmdMesseger->readBinArg<int>();

  targetDistance = (FOCAL_LENGHT * CONE_REAL_HEIGHT * IMAGE_PIXEL_HEIGHT) / (h * SENSOR_HEIGHT);
}

void onRecvUnknownCommand(CmdMessenger *cmdMesseger)
{
  // Algum buzzer futuramente
}

