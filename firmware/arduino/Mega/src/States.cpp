#include "States.h"
#include "Variables.h"
#include "Constants.h"
#include "CommandHandlers.h"
#include "Macros.h"
#include "Pins.h"
#include "PIDHelper.h"

float rotateCameraIncrement = 1 / 90.0;

void changeState(void (*nextState)(unsigned long))
{
    previousState = state;
    state = nextState;
}

void reset(unsigned long deltaTime)
{
    rPiCmdMessenger.sendCmdStart(MessageCodesRPi::kRPiLog);
    rPiCmdMessenger.sendCmdArg("reset");
    rPiCmdMessenger.sendCmdEnd();

    linearSpeed = 0;
    steeringServoPosition = 0;
    cameraServoPosition = 0;
    changeState(idle);
}

void idle(unsigned long deltaTime)
{
    /*
    rPiCmdMessenger.sendCmdStart(MessageCodesRPi::kRPiLog);
    rPiCmdMessenger.sendCmdArg("idle");
    rPiCmdMessenger.sendCmdEnd();
*/
    linearSpeed = 0;

    setManual();
    actuatorsWrite = ExecutionFlags::kAll;
}

void search(unsigned long deltaTime)
{
    rPiCmdMessenger.sendCmdStart(MessageCodesRPi::kRPiLog);
    rPiCmdMessenger.sendCmdArg("search");
    rPiCmdMessenger.sendCmdEnd();

    //Serial.println("search");
    // Como sempre vamos começar muito longe dos objetivos
    // seria muito difícil a câmera detectar o cone.
    // Então inicialmente vamos avaliar a distancia e direção
    // usando o MPU.
    //
    // Pra isso precisamos sobreescrever os valores que estamos
    // recebendo do Raspberry Pi via porta serial.
    targetDistance = currentTransform.position.distance(currentTarget.x, currentTarget.y);
    targetDirection = DIRECTION(currentTarget, currentTransform.position);

    // Usa atan2 para obter valores sempre entre [-π ; +π] radianos.
    targetDirection = CORRECT_DIRECTION(targetDirection);

    // Quando estivermos suficientemente próximos (REFINED_SEARCH_DISTANCE)
    // troca pra o estado refinedSearch.
    if (targetDistance < REFINED_SEARCH_DISTANCE)
    {
        changeState(refinedSearch);
        rPiCmdMessenger.sendCmd(MessageCodesRPi::kStartDetection);
    }
    actuatorsWrite = ExecutionFlags::kAll;
}

void refinedSearch(unsigned long deltaTime)
{
    rPiCmdMessenger.sendCmdStart(MessageCodesRPi::kRPiLog);
    //rPiCmdMessenger.sendCmdArg("refinedSearch");
    rPiCmdMessenger.sendCmdArg(targetDistance);
    rPiCmdMessenger.sendCmdArg(targetDirection);
    rPiCmdMessenger.sendCmdEnd();
    /*
        //Serial.println("refinedSearch");
        if (targetDistance < GOAL_THRESHOLD)
        {
                changeState(targetFound);
                rPiCmdMessenger.sendCmd(MessageCodesRPi::kPauseDetection);
                return;
        }

        int detectedCount = 0;
        if(!sonicArray.update(&detectedCount))
        {
                return;
        }

        float preferrableDirection = sonicArray.obstacleAvoidance();
        targetDirection = preferrableDirection;
        */
}

int targetCount = 0;
void targetFound(unsigned long deltaTime)
{
    rPiCmdMessenger.sendCmdStart(MessageCodesRPi::kRPiLog);
    rPiCmdMessenger.sendCmdArg("targetFound");
    rPiCmdMessenger.sendCmdEnd();
    targetCount++;
    esc.write(ESC_ZERO);

    if (currentTarget.signal)
    {
        digitalWrite(LED_SIGNAL_PIN, HIGH);
        delay(1000);
        digitalWrite(LED_SIGNAL_PIN, LOW);
    }
    // Vira a direção totalmente para a esquerda anda de ré.
    // Idealmente isso vai colocar o robô mais ou menos na direção
    // do proximo objetivo
    steeringServo.write(0);
    esc.write(ESC_MAX_BACKWARD);
    delay(1000);

    if (targetCount == targets.size())
    {
        changeState(idle);
    }
    else
    {
        changeState(search);
        currentTarget = targets.get(targetCount);
    }

    actuatorsWrite = ExecutionFlags::kNone;
}

void rotateCamera(unsigned long deltaTime)
{
    /*
    rPiCmdMessenger.sendCmdStart(MessageCodesRPi::kRPiLog);
    rPiCmdMessenger.sendCmdArg("rotateCamera");
    rPiCmdMessenger.sendCmdEnd();
    */
    linearSpeed = 0;
    cameraServoPosition += rotateCameraIncrement;
    if (abs(cameraServoPosition) >= 1)
    {
        rotateCameraIncrement *= -1;
    }
    actuatorsWrite = ExecutionFlags::kAll;
}