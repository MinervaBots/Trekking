#include "CommandHandlers.h"
#include "Variables.h"
#include "Constants.h"
#include "States.h"

unsigned long targetLostStartTime;


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
  if(targetLostStartTime != 0 && state != refinedSearch)
  {
    targetLostStartTime = 0;
    changeState(refinedSearch);
  }

  linearSpeedLock = 1;
  targetDirection = cmdMesseger->readFloatArg();
  int h = cmdMesseger->readInt16Arg();
  
  if(h != 0)
  {
    targetDistance = (FOCAL_LENGHT * CONE_REAL_HEIGHT * IMAGE_PIXEL_HEIGHT) / (h * SENSOR_HEIGHT);
  }
  else
  {
    targetDistance = 0;
  }
}

void onRecvTargetLost(CmdMessenger *cmdMesseger)
{
  if(targetLostStartTime == 0)
  {
    targetLostStartTime = millis();
  }
  else if(millis() - targetLostStartTime > TARGET_LOST_THRESHOLD && state != rotateCamera)
  {
    changeState(rotateCamera);
    rPiCmdMessenger.sendCmdStart(MessageCodesRPi::kRPiLog);
    rPiCmdMessenger.sendCmdArg("changeState(rotateCamera)");
    rPiCmdMessenger.sendCmdEnd();
  }
}

void onRecvUnknownCommand(CmdMessenger *cmdMesseger)
{
  rPiCmdMessenger.sendCmdStart(MessageCodesRPi::kRPiLog);
  rPiCmdMessenger.sendCmdArg("Mensagem desconhecida");
  rPiCmdMessenger.sendCmdEnd();
}