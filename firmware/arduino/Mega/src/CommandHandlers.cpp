#include "CommandHandlers.h"
#include "Variables.h"
#include "Constants.h"
#include "States.h"
#include "Utils.h"

unsigned long targetLostStartTime;

void attachHandlers()
{
    rPiCmdMessenger.attach(MessageCodesRPi::kTargetFound, onRecvTargetFound);
    rPiCmdMessenger.attach(MessageCodesRPi::kTargetLost, onRecvTargetLost);
    rPiCmdMessenger.attach(MessageCodesRPi::kStopEvent, onStopEvent);
    rPiCmdMessenger.attach(onRecvUnknownCommand);

    mpuCmdMessenger.attach(MessageCodesMPU::kUpdateTransform, onUpdateTransform);
    mpuCmdMessenger.attach(MessageCodesMPU::kMpuLog, onRecvMpuLog);
    mpuCmdMessenger.attach(onRecvUnknownCommand);
}

void onStopEvent(CmdMessenger *cmdMesseger)
{
    linearSpeed = 0;
    esc.write(ESC_ZERO);
    changeState(reset);
}

void onRecvTargetFound(CmdMessenger *cmdMesseger)
{
    if (targetLostStartTime != 0 && state != refinedSearch)
    {
        targetLostStartTime = 0;
        changeState(refinedSearch);
    }

    linearSpeedLock = 1;
    float newDirection = cmdMesseger->readFloatArg();
    targetDirection = lerp(targetDirection, newDirection, 0.8);

    float newDistance = cmdMesseger->readFloatArg();
    targetDistance = lerp(targetDistance, newDistance, 0.1);
}

void onRecvTargetLost(CmdMessenger *cmdMesseger)
{
    if (targetLostStartTime == 0)
    {
        targetLostStartTime = millis();
    }
    else if (millis() - targetLostStartTime > TARGET_LOST_THRESHOLD && state != rotateCamera)
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
    rPiCmdMessenger.sendCmdArg("Nenhum handler registrado para essa mensagem");
    rPiCmdMessenger.sendCmdEnd();
}

void onUpdateTransform(CmdMessenger *cmdMesseger)
{
    currentTransform.position.x = cmdMesseger->readBinArg<float>();
    currentTransform.position.y = cmdMesseger->readBinArg<float>();
    currentTransform.heading = cmdMesseger->readBinArg<float>();
}

void onRecvMpuLog(CmdMessenger *cmdMesseger)
{
    rPiCmdMessenger.sendCmdStart(MessageCodesRPi::kRPiLog);
    while (cmdMesseger->available())
    {
        rPiCmdMessenger.sendCmdBinArg(cmdMesseger->readStringArg());
    }
    rPiCmdMessenger.sendCmdEnd();
}
