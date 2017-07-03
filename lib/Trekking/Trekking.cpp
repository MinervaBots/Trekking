#include "Trekking.hpp"
#include <Arduino.h>

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
  m_Odometry = m_pTrekkingSensoring->getInput();
  if(!m_IsRunning)
  {
    return;
  }
  Serial.println("update");
  //delay(500);

  unsigned long deltaTime = millis() - m_LastIterationTime;
  (this->*m_CurrentMode)(deltaTime);
  m_LastIterationTime = millis();
  //delay(250);
}


/*----|Modos de operação|-----------------------------------------------------*/
void Trekking::rotateToTarget(unsigned long deltaTime)
{
  Serial.print("rotateToTarget: ");
  Serial.println(m_CurrentTargetId);
  auto target = m_Targets.get(m_CurrentTargetId);
  auto currentPosition = m_pTrekkingSensoring->getPosition();

  float heading = currentPosition.getHeading();
  float desiredHeading = atan2((target.getY() - currentPosition.getY()),(target.getX() - currentPosition.getX()));
  float headingError = desiredHeading - heading;
  //headingError = atan2(sin(headingError), cos(headingError));

  Serial.print(desiredHeading);
  Serial.print(" - ");
  Serial.print(heading);
  Serial.print(" = ");
  Serial.println(headingError);


  if(abs(headingError) > 0.02)
  {
    float angularVelocity = 0.7 * headingError/abs(headingError);
    Serial.print("angularVelocity: ");
    Serial.println(angularVelocity);
    m_pMotorController->move(0, angularVelocity);//m_pSystemController->run(headingError));
    return;
  }

  /*
  Depois que atingirmos a rotação que queriamos, lê o magnetometro e salva o
  valor na linha de referência que vai ser usada para o PID no estado 'search'.
  */
  m_ReferenceLine = desiredHeading;// m_Odometry.getU();
  Serial.print("m_ReferenceLine: ");
  Serial.println(m_ReferenceLine);
  /*
  Se estamos indo parao ultimo objetivo, devemos ativar os sensores de ultrassom
  e os sensores de cor.
  */
  if(m_CurrentTargetId == 2)
  {

  }

  m_CurrentMode = &Trekking::search;
}

void Trekking::search(unsigned long deltaTime)
{
  Serial.println("search");
  auto target = m_Targets.get(m_CurrentTargetId);
  auto distanceToTarget = distance(target);

  if(distanceToTarget > 0.2)
  {
    auto currentPosition = m_pTrekkingSensoring->getPosition();
    float heading = currentPosition.getHeading();
    /*

    auto plan = Vector2<float>();
    plan.setX(currentPosition.getX() - target.getX());
    plan.setY(currentPosition.getY() - target.getY());

    float linearVelocity = cos(-heading) * plan.getX() + sin(-heading) * plan.getY();
    float angularVelocity = (sin(-heading) * plan.getX() - cos(-heading) * plan.getY()) / m_ControlPointDistance;
    m_pMotorController->move(linearVelocity, angularVelocity);
    */


    float lineError = (m_ReferenceLine - heading); //m_Odometry.getU()) / PI;
    float angularVelocity = lineError; //m_pSystemController->run(lineError);
    m_pMotorController->move(0.5, angularVelocity);

    Serial.print("lineError: ");
    Serial.print(lineError);
    Serial.print("\tangularVelocity: ");
    Serial.print(angularVelocity);
    Serial.print("\tm_CurrentTargetId: ");
    Serial.println(m_CurrentTargetId);

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
      else if(m_CurrentTargetId == 2)
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

  /*
  Se já estamos a uma distância mínima do nosso objetivo (garante que não
  tenham problemas e nunca mude de estado) muda para 'refinedSearch'
  */
  m_CurrentMode = &Trekking::buzzer;
  //this->stop();
  Serial.println("POSIÇÃO DESEJADA ALCANÇADA!");
  m_StartTimeInRefinedSearch = millis();
}

void Trekking::refinedSearch(unsigned long deltaTime)
{
  Serial.println("refinedSearch");
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
  /*
  Temos que inicializar uam matriz com tamanho um suficientemente grande para
  incluir todos os obstáculos, o objetivo e o robô.

  [TODO]: Estudar a melhor maneira de fazer isso. Devido as limitações de
  memória do Arduino, não é ideal criar uma matriz do tamanho do campo. Devido
  a isso a matemática fica um pouco mais complicada já que as posições vão ter
  um offset.
  Outra "dificuldade" é converter o posicionamento "real" para as posições na
  matriz. Além das posições dela
  */
  //m_Map = BitMatrix<8, 8>(false);

  /*
  Aqui lemos o ultrassom e calculamos a posição que devemos colocar o obstaculo
  no mapa
  */
}

void Trekking::standBy(unsigned long deltaTime)
{
  stop();
  if(m_OperationMode)
  {

  }
  /*
  if(operation_mode_switch == AUTO_MODE) {
		is_manual_message_sent = false;
		if(!is_auto_message_sent){
			log.debug("mode switch", "auto ");
			is_auto_message_sent = true;
		}
		if(init_button) {
			log.debug("init button", init_button);
			reset();
			if(checkSensors()) {
				operation_mode = &Trekking::search;
				startTimers();
			} else {
				log.error("sensors", "sensors not working as expected");
			}
		}
	}	else{
		is_auto_message_sent = false;
		if(!is_manual_message_sent){
			log.debug("mode switch", "manual ");
			is_manual_message_sent = true;
		}
		if(current_command != ' ') {
			Robot::useCommand(current_command);
			log.debug("using command", current_command);
		}
	}
  */
}

void Trekking::buzzer(unsigned long deltaTime)
{
  m_pMotorController->stop();
  if(m_CurrentTargetId == 2)
  {
    finish(deltaTime);
    return;
  }
  turnBuzzerOn();
  delay(5000);
  turnBuzzerOff();

  m_CurrentTargetId++;
  m_CurrentMode = &Trekking::rotateToTarget;
}

void Trekking::finish(unsigned long deltaTime)
{
  turnBuzzerOn();
  delay(5000);
  turnBuzzerOff();

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
