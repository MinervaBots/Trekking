#include "States.h"
#include "Variables.h"
#include "Constants.h"
#include "CommandHandlers.h"
#include "Macros.h"

void idle(unsigned long deltaTime)
{
  linearSpeed = 0;
  steeringServoPosition = 0;
  cameraServoPosition = 0;
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
  targetDistance = currentTransform.position.distance(currentTarget->x, currentTarget->y);
  targetDirection = DIRECTION((*currentTarget), currentTransform.position);

  // Usa atan2 para obter valores sempre entre [-π ; +π] radianos.
  targetDirection = CORRECT_DIRECTION(targetDirection);

  // Quando estivermos suficientemente próximos (REFINED_SEARCH_DISTANCE)
  // troca pra o estado refinedSearch.
  if (targetDistance < REFINED_SEARCH_DISTANCE)
  {
    state = refinedSearch;
    rPiCmdMessenger.sendCmd(startDetection);
  }
}

void refinedSearch(unsigned long deltaTime)
{
  if (targetDistance < GOAL_THRESHOLD)
  {
    state = targetFound;
    rPiCmdMessenger.sendCmd(pauseDetection);
    return;
  }

  float avoidMultiplyer;
  if (ultrassonicArray.evaluate(&avoidMultiplyer))
  {
    if (avoidMultiplyer != 0)
    {
      targetDirection = STEERING_SERVO_LIMIT * avoidMultiplyer;
      targetDirection = CORRECT_DIRECTION(targetDirection);
    }
    else
    {
      // Caso tenha um obstáculo detectado, mas não conseguimos calcular uma direção
      // Volta um pouco e tenta novamente
      targetDirection = 0;
      linearSpeed = ESC_MAX_BACKWARD;
    }
  }
}

int targetCount = 0;
void targetFound(unsigned long deltaTime)
{
  targetCount++;
  esc.write(ESC_ZERO);

  // Sinaliza
  delay(1000);
  // Desliga o sinal

  // Vira a direção totalmente para a esquerda anda de ré.
  // Idealmente isso vai colocar o robô mais ou menos na direção
  // do proximo objetivo
  steeringServo.write(0);
  esc.write(ESC_MAX_BACKWARD);
  delay(1000);


  if (targetCount == targets.size())
  {
    state = idle;
  }
  else
  {
    state = search;
  }
}

