#include "States.h"
#include "Variables.h"
#include "Constants.h"

void idle(unsigned long lastRun)
{
  linearSpeed = 0;
  steeringServoPosition = 0;
  cameraServoPosition = 0;
}

void search(unsigned long lastRun)
{
  // Como sempre vamos começar muito longe dos objetivos
  // seria muito difícil a câmera detectar o cone.
  // Então inicialmente vamos avaliar a distancia e direção
  // usando o MPU.
  //
  // Pra isso precisamos sobreescrever os valores que estamos
  // recebendo do Raspberry Pi via porta serial.
  targetDistance = currentTransform.position.distance(currentTarget->x, currentTarget->y);
  targetDirection = atan((currentTarget->y - currentTransform.position.y) / (currentTarget->x - currentTransform.position.x));

  // Usa atan2 para obter valores sempre entre [-π ; +π] radianos.
  targetDirection = atan2(sin(targetDirection), cos(targetDirection));

  // Quando estivermos suficientemente próximos (REFINED_SEARCH_DISTANCE)
  // troca pra o estado refinedSearch.
  if (targetDistance < REFINED_SEARCH_DISTANCE)
  {
    state = refinedSearch;
  }
}

void refinedSearch(unsigned long lastRun)
{
  if(targetDistance < GOAL_THRESHOLD)
  {
    state = targetFound;
    return;
  }

  float obsDistX, obsDistY = 0;
  float meanDirection;
  for(int i = 0; i < ULTRASSONIC_COUNT; i++)
  {
    auto result = ultrassonicArray.evaluate(i);
    if(result.distance > 10000)
    {
      // Nada foi detectado
      continue;
    }
    obsDistX += result.distance * sin(result.direction);
    obsDistY += result.distance * cos(result.direction);
    meanDirection += result.direction;
  }
  obsDistX /= ULTRASSONIC_COUNT;
  obsDistY /= ULTRASSONIC_COUNT;
  meanDirection /= ULTRASSONIC_COUNT;
  meanDirection = atan2(sin(meanDirection), cos(meanDirection));
  
  // Se o obstáculo não estiver na direção que estamos indo ignora
  if(abs(meanDirection - targetDirection) > PI/4)
  {
    return;
  }

  if(meanDirection > 0)
  {
    targetDirection -= PI/2;
  }
  else
  {
    targetDirection += PI/2;
  }
    
  targetDirection = atan2(sin(targetDirection), cos(targetDirection));
}

void targetFound()
{
  // Sinaliza
  delay(1000);
  // Desliga o sinal

  // Vira a direção totalmente para a esquerda anda de ré.
  // Idealmente isso vai colocar o robô mais ou menos na direção
  // do proximo objetivo
  steeringServoPosition = 0;
  linearSpeed = -1;
  delay(1000);

  state = search;
}

