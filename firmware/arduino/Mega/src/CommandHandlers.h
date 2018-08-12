#ifndef COMMAND_HANDLERS_H
#define COMMAND_HANDLERS_H

#include <CmdMessenger.h>

enum LogMessageSubCodes
{
    kInfo,
    kWarning,
    kError
};

enum MessageCodesRPi
{
    kRPiLog = 0,
    kStartDetection,
    kPauseDetection,
    kTargetFound,
    kTargetLost,
    kStopEvent,
    kTemperature,
};

enum MessageCodesMPU
{
    kMpuLog,
    kUpdateTransform,
    kSetPosition,
};

void attachHandlers();
void onRecvTargetFound(CmdMessenger *cmdMesseger);
void onRecvTargetLost(CmdMessenger *cmdMesseger);
void onRecvUnknownCommand(CmdMessenger *cmdMesseger);
void onStopEvent(CmdMessenger *cmdMesseger);
void onTemperatureUpdateEvent(CmdMessenger *cmdMesseger);


void onUpdateTransform(CmdMessenger *cmdMesseger);
void onRecvMpuLog(CmdMessenger *cmdMesseger);


#endif //COMMAND_HANDLERS_H
