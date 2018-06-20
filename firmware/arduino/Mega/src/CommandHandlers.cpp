#include "CommandHandlers.h"
#include "Variables.h"
#include "Constants.h"
#include "States.h"

unsigned long targetLostStart;


void onStopEvent(CmdMessenger *cmdMesseger)
{
  changeState(idle);
  rPiCmdMessenger.sendCmdStart(MessageCodesRPi::kRPiLog);
  rPiCmdMessenger.sendCmdBinArg("Parou");
  rPiCmdMessenger.sendCmdEnd();
}

void onRecvMpuData(CmdMessenger *cmdMesseger)
{
  currentTransform.position.x = cmdMesseger->readBinArg<float>();
  currentTransform.position.y = cmdMesseger->readBinArg<float>();
  currentTransform.heading = cmdMesseger->readBinArg<float>();
}

void onRecvMpuLog(CmdMessenger *cmdMesseger)
{
  rPiCmdMessenger.sendCmdStart(MessageCodesRPi::kRPiLog);
  while(cmdMesseger->available())
  {
    rPiCmdMessenger.sendCmdBinArg(cmdMesseger->readStringArg());
  }
  rPiCmdMessenger.sendCmdEnd();
}

void onRecvTargetFound(CmdMessenger *cmdMesseger)
{
  cmdMesseger->sendCmdStart(MessageCodesRPi::kRPiLog);
  cmdMesseger->sendCmdArg("Alvo localizado");
  cmdMesseger->sendCmdEnd();
/*
  if(targetLostStart != 0)
  {
    targetLostStart = 0;
    changeState(refinedSearch);
  }
*/
  linearSpeedLock = 1;
  targetDirection = cmdMesseger->readBinArg<double>();
  targetDirectionFiltered.add(targetDirection);
  
  //int x = cmdMesseger->readBinArg<int>();
  //int y = cmdMesseger->readBinArg<int>();
  //int w = cmdMesseger->readBinArg<int>();
  int h = cmdMesseger->readBinArg<int>();
  targetDistanceFiltered.add((FOCAL_LENGHT * CONE_REAL_HEIGHT * IMAGE_PIXEL_HEIGHT) / (h * SENSOR_HEIGHT));
}

void onRecvTargetLost(CmdMessenger *cmdMesseger)
{
  // Considera que o alvo foi perdido por problemas da deteção
  // mas ele ainda está no campo de visão, logo apenas diminui
  // a velocidade aos poucos

  linearSpeedLock *= 0.99;

  if(targetLostStart == 0)
  {
    targetLostStart = millis();
  }
  else if(millis() - targetLostStart > TARGET_LOST_THRESHOLD)
  {
    changeState(rotateCamera);
  }
}

void onRecvUnknownCommand(CmdMessenger *cmdMesseger)
{
  rPiCmdMessenger.sendCmdStart(MessageCodesRPi::kRPiLog);
  rPiCmdMessenger.sendCmdArg("Mensagem desconhecida");
  rPiCmdMessenger.sendCmdEnd();
}