#ifndef COMMAND_HANDLERS_H
#define COMMAND_HANDLERS_H

#include <CmdMessenger.h>

enum MessageCodesRPi
{
  kRPiLog,
  kTargetFound,
  kTargetLost,
  kStartDetection,
  kPauseDetection,
};

enum MessageCodesMPU
{
  kMpuData,
  kMpuLog,
  kSetPosition
};

void onRecvMpuData(CmdMessenger *cmdMesseger);
void onRecvMpuLog(CmdMessenger *cmdMesseger);
void onRecvTargetFound(CmdMessenger *cmdMesseger);
void onRecvTargetLost(CmdMessenger *cmdMesseger);
void onRecvUnknownCommand(CmdMessenger *cmdMesseger);

#endif //COMMAND_HANDLERS_H
