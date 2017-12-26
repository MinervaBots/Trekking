#ifndef COMMAND_HANDLERS_H
#define COMMAND_HANDLERS_H

#include <CmdMessenger.h>

enum
{
  info,
  error,
  mpuData,
  targetData,
  startDetection,
  pauseDetection
};

void onRecvMpuData(CmdMessenger *cmdMesseger);
void onRecvTargetData(CmdMessenger *cmdMesseger);
void onRecvUnknownCommand(CmdMessenger *cmdMesseger);

#endif //COMMAND_HANDLERS_H
