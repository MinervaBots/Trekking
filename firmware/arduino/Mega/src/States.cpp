#include "States.h"
#include "Variables.h"
#include "Constants.h"
#include "CommandHandlers.h"
#include "Macros.h"
#include "Pins.h"


void changeState(void (*nextState)(unsigned long))
{
  previousState = state;
  state = nextState;
}

void reset(unsigned long deltaTime)
{
  linearSpeed = 0;
  steeringServoPosition = 0;
  cameraServoPosition = 0;
  changeState(idle);
}

void idle(unsigned long deltaTime)
{
  linearSpeed = 0;
  computePid = ExecutionFlags::kNone;
  actuatorsWrite = ExecutionFlags::kAll;
}

void search(unsigned long deltaTime)
{
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
  computePid = ExecutionFlags::kAll;
  actuatorsWrite = ExecutionFlags::kAll;
}

void refinedSearch(unsigned long deltaTime)
{
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

  computePid = ExecutionFlags::kAll;
  actuatorsWrite = ExecutionFlags::kAll;
}

int targetCount = 0;
void targetFound(unsigned long deltaTime)
{
  targetCount++;
  esc.write(ESC_ZERO);

  if(currentTarget.signal)
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
  
  computePid = ExecutionFlags::kNone;
  actuatorsWrite = ExecutionFlags::kNone;
}

void rotateCamera(unsigned long deltaTime)
{
  esc.write(ESC_ZERO);
  steeringServoPosition += 1;
  if(steeringServoPosition > 180)
  {
    steeringServoPosition = 0;
  }
  delay(20);

  computePid = ExecutionFlags::kNone;
  actuatorsWrite = ExecutionFlags::kCamera;
}