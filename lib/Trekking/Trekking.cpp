#include "Trekking.hpp"
#include "MathHelper.h"
#include <Arduino.h>

const long DistanceTimeCheck = 250;

void Trekking::setup()
{
  m_CurrentMode = &Trekking::standBy;
}

void Trekking::start()
{
  emergency();
  delay(1500);
  m_CurrentMode = &Trekking::rotateToTarget;
  m_StartTime = millis();
  m_LastIterationTime = m_StartTime;
  m_IsRunning = true;
  m_LastDistance = 9999;
  m_LastDistanceTimeCheck = DistanceTimeCheck;
  m_pMotorController->reset();

  IRotation = 0;
  KpRotation = 4;
  KiRotation = 0.01; //0.01;
  KdRotation = 5;

  ISearch = 0;
  KpSearch = 5;
  KiSearch = 0.004; //0.01;
  KdSearch = 1;
}

void Trekking::stop()
{
  m_StopTime = millis();
  m_IsRunning = false;
  m_pMotorController->stop();
}

void Trekking::emergency()
{
  stop();
  m_CurrentMode = &Trekking::standBy;
}

void Trekking::update()
{
  if(m_IsRunning && !m_IsPaused)
  {
    m_Odometry = m_pTrekkingSensoring->getInput();
    (this->*m_CurrentMode)(millis() - m_LastIterationTime);
  }
  m_LastIterationTime = millis();
}

/*----|Modos de operação|-----------------------------------------------------*/
void Trekking::rotateToTarget(unsigned long deltaTime)
{
  /*
  Serial.print("rotateToTarget: ");
  Serial.println(m_CurrentTargetId);
  */
  auto target = m_Targets.get(m_CurrentTargetId);
  auto currentPosition = m_pTrekkingSensoring->getPosition();

  float desiredHeading = atan2(target.getY() - currentPosition.getY(), target.getX() - currentPosition.getX());
  float headingError = desiredHeading - currentPosition.getHeading();
  headingError = atan2(sin(headingError), cos(headingError));

  if(abs(headingError) > 0.01)
  {
    IRotation += headingError * KiRotation * deltaTime;
    float angularVelocity = 2 * headingError/abs(headingError);//headingError * KpRotation + IRotation + KdRotation * (headingError - lastErrorRotation)/ deltaTime;
    lastErrorRotation = headingError;
    //angularVelocity = constrain(angularVelocity, -4, 4);
    m_pMotorController->move(0, angularVelocity);//m_pSystemController->run(headingError));
    return;
  }

  m_pMotorController->stop();
  delay(250);
  /*
  Depois que atingirmos a rotação que queriamos, lê o magnetometro e salva o
  valor na linha de referência que vai ser usada para o PID no estado 'search'.
  */
  m_ReferenceLine = currentPosition.getHeading();
  /*
  Serial.print("m_ReferenceLine: ");
  Serial.println(m_ReferenceLine);
  */
  /*
  Se estamos indo parao ultimo objetivo, devemos ativar os sensores de ultrassom
  e os sensores de cor.
  */
  if(m_CurrentTargetId == m_Targets.size() - 1)
  {

  }

  IRotation = 0;
  m_LastDistance = 99999;
  m_LastDistanceTimeCheck = DistanceTimeCheck;
  m_StartTimeOnSearch = millis();
  m_CurrentMode = &Trekking::search;
}

void Trekking::search(unsigned long deltaTime)
{
  //Serial.println("search");
  auto target = m_Targets.get(m_CurrentTargetId);
  auto distanceToTarget = distance(target);

  //Serial.println(distanceToTarget);
  //if(distanceToTarget > 0.1)
  m_LastDistanceTimeCheck -= deltaTime;
  if(m_LastDistanceTimeCheck < 0)
  {
    m_LastDistance = distanceToTarget;
    m_LastDistanceTimeCheck = DistanceTimeCheck;
  }
/*
  Serial.print("m_LastDistance: ");
  Serial.print(m_LastDistance);
  Serial.print("\tdistanceToTarget: ");
  Serial.print(distanceToTarget);
  Serial.print("\tm_LastDistanceTimeCheck: ");
  Serial.println(m_LastDistanceTimeCheck);
*/
  if(m_LastDistance >= distanceToTarget && distanceToTarget > 0.1)
  {
    auto currentPosition = m_pTrekkingSensoring->getPosition();
    float heading = currentPosition.getHeading();

    float lineError = (m_ReferenceLine - heading);
    lineError = atan2(sin(lineError), cos(lineError));

    ISearch += lineError * KiSearch * deltaTime;
    float angularVelocity = lineError * KpSearch + ISearch;
    angularVelocity = constrain(angularVelocity, -3, 3);

    if(distanceToTarget < 1)
    {
      // Rampa de desaceleração
      m_CurrentLinearVelocity = (distanceToTarget/1.5) + 0.3;
      //m_CurrentLinearVelocity = lerp(0, 1, distanceToTarget + 0.5); nao usar esse
    }
    else
    {
      // Rampa de aceleração
      m_CurrentLinearVelocity = constrain(lerp(0, 1, (millis() - m_StartTimeOnSearch) / 2000.0), 0, 1);
    }
    m_pMotorController->move(m_CurrentLinearVelocity, angularVelocity);

    /*
    Se estivermos perto o suficiente, já podemos começar a procurar os cones
    */
    /*
    if(distanceToTarget > 3)
      return;

    auto objectsVector = m_Odometry.getV();
    float distance = objectsVector.getY();

    if(distance > 0)
    {
      if(m_Odometry.getT())
      {
        // Opa! Já chegamos no objetivo
        m_CurrentMode = &Trekking::buzzer;
      }
      else if(m_CurrentTargetId == m_Targets.size() - 1)
      {
        /*
        Se não chegamos no objetivo, e estamos indo na direção do ultimo, isso
        é um obstaculo.
        *
        m_CurrentMode = &Trekking::avoidObstacles;
      }
      else
      {
        /*
        É um cone, mas ainda estamos muito longe pra ver a base branca.
        Muda pra refinedSearch e deixa que ele direcione até o cone.
        *
        m_CurrentMode = &Trekking::refinedSearch;
        m_StartTimeInRefinedSearch = millis();
      }
    }
    */
    return;
  }


  for(; m_CurrentLinearVelocity >= 0; m_CurrentLinearVelocity -= 0.1)
  {
    m_pMotorController->move(m_CurrentLinearVelocity, 0);
    delay(30);
  }
  m_CurrentLinearVelocity = 0;
  /*
  Se já estamos a uma distância mínima do nosso objetivo (garante que não
  tenham problemas e nunca mude de estado) muda para 'refinedSearch'
  */

  m_CurrentMode = &Trekking::buzzer;
  m_LastDistance = 9999;
  /*
  this->stop();
  Serial.println("POSIÇÃO DESEJADA ALCANÇADA!");
  */
  m_StartTimeInRefinedSearch = millis();

  Serial.println("distance: ");
  Serial.println(distanceToTarget);
}

void Trekking::refinedSearch(unsigned long deltaTime)
{
  //Serial.println("refinedSearch");
  if(!m_Odometry.getT())
  {
    auto targetVector = m_Odometry.getV();
    float distance = targetVector.getY();
    if(distance != -1)
    {
      float direction = targetVector.getX();

      float angularVelocity = m_pSystemController->run(direction);
      m_pMotorController->move(0.5, angularVelocity);
    }
    else
    {
      if(millis() - m_StartTimeInRefinedSearch < m_MaxTimeInRefinedSearch)
      {
        /*
        Se ainda não ultrapassamos um tempo limite (digamos, que o suficiente
        pra completar uma volta), gira em torno do próprio eixo e tenta localizar
        o objetivo.
        */
        m_pMotorController->move(0, -1);
        return;
      }
      /*
      A ideia é que se não localizamos o alvo girando no próprio eixo, devemos
      ir abrindo cada vez mais a área de busca.

      Então se move fazendo circulos cada vez maiores.
      */
      m_pMotorController->move(0.7, -1);
    }
    return;
  }
  m_CurrentMode = &Trekking::buzzer;
}

void Trekking::avoidObstacles(unsigned long deltaTime)
{
}

void Trekking::standBy(unsigned long deltaTime)
{
  stop();
}

void Trekking::buzzer(unsigned long deltaTime)
{
  m_pMotorController->stop();
  //auto target = m_Targets.get(m_CurrentTargetId);
  //m_pTrekkingSensoring->setPosition(target.getX(), target.getY(), m_pTrekkingSensoring->getPosition().getHeading());

  if(m_CurrentTargetId == m_Targets.size() - 1)
  {
    finish(deltaTime);
    return;
  }
  turnBuzzerOn();
  delay(2000);
  turnBuzzerOff();

  m_CurrentTargetId++;
  m_CurrentMode = &Trekking::rotateToTarget;
  ISearch = 0;
}

void Trekking::finish(unsigned long deltaTime)
{
  turnBuzzerOn();
  delay(5000);
  turnBuzzerOff();

  stop();
  m_CurrentTargetId = 0;
  // Sobrescreve o estado que deve ser executado na próxima iteração
  m_CurrentMode = &Trekking::standBy;
}
/*----------------------------------------------------------------------------*/


/*----|Funções operacionais|--------------------------------------------------*/
void Trekking::turnBuzzerOn()
{
  digitalWrite(m_BuzzerPin, HIGH);
}

void Trekking::turnBuzzerOff()
{
  digitalWrite(m_BuzzerPin, LOW);
}
/*----------------------------------------------------------------------------*/
