#ifndef COMMAND_HANDLERS_H
#define COMMAND_HANDLERS_H

#include <CmdMessenger.h>

enum MessageCodesRPi
{
  kRPiLog = 0,
  kTargetFound,
  kTargetLost,
  kStartDetection,
  kPauseDetection,
  kStopEvent,
};

enum MessageCodesMPU
{
  kMpuData,
  kMpuLog,
  kSetPosition
};

void attachHandlers();
void onRecvMpuData(CmdMessenger *cmdMesseger);
void onRecvMpuLog(CmdMessenger *cmdMesseger);
void onRecvTargetFound(CmdMessenger *cmdMesseger);
void onRecvTargetLost(CmdMessenger *cmdMesseger);
void onRecvUnknownCommand(CmdMessenger *cmdMesseger);
void onStopEvent(CmdMessenger *cmdMesseger);

#endif //COMMAND_HANDLERS_H
