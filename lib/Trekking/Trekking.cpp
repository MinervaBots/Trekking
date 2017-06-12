#include "Trekking.hpp"
#include <Arduino.h>

void Trekking::setup()
{
}

void Trekking::start()
{
  emergency();
  delay(1500);
  m_StartTime = millis();
  m_LastIterationTime = m_StartTime;
  m_IsRunning = true;
}

void Trekking::stop()
{
  m_StopTime = millis();
  m_IsRunning = false;
}

void Trekking::emergency()
{
  stop();
  m_CurrentMode = &Trekking::standBy;
}

void Trekking::update()
{
  if(!m_IsRunning)
    return;

  m_Odometry = m_pTrekkingSensoring->getInput();


  unsigned long deltaTime = millis() - m_LastIterationTime;
  (this->*m_CurrentMode)(deltaTime);
  m_LastIterationTime = millis();
}


/*----|Modos de operação|-----------------------------------------------------*/
void Trekking::rotateToTarget(unsigned long deltaTime)
{
  auto target = m_Targets.get(m_CurrentTargetId);
  auto currentPosition = m_pTrekkingSensoring->getPosition();

  float heading = currentPosition.getHeading();
  float desiredHeading = atan((target.getY() - currentPosition.getY()) / (target.getX() - currentPosition.getX()));

  float headingError = desiredHeading - heading;
  headingError = atan2(sin(headingError), cos(headingError));

  if(abs(headingError) > 0.5)
  {
    float angularVelocity = m_pSystemController->run(headingError);
    m_pMotorController->move(0, angularVelocity);
    return;
  }

  /*
  Depois que atingirmos a rotação que queriamos, lê o magnetometro e salva o
  valor na linha de referência que vai ser usada para o PID no estado 'search'.
  */
  m_ReferenceLine = m_Odometry.getU();

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
  auto target = m_Targets.get(m_CurrentTargetId);
  if(distance(target) > 0.1)
  {
    /*
    Se já estamos indo para o ultimo objetivo, temos que ficar atentos a
    obstaculos
    */
    if(m_CurrentTargetId == 2)
    {
      auto objectsVector = m_Odometry.getV();
      // Alguma coisa foi detectada
      if(objectsVector.getX() != -1)
      {
        if(m_Odometry.getT())
        {
          /*
          Objetivo alcançado.
          Acredito que nunca vai fazer isso, porque na prática vamos encontrar
          obstaculos antes do objetivo, mas vai que...
          */
          m_CurrentMode = &Trekking::finish;
        }
        else
        {
          m_CurrentMode = &Trekking::avoidObstacles;
        }
        return;
      }
    }

    /*
    auto currentPosition = m_pTrekkingSensoring->getPosition();
    float heading = currentPosition.getHeading();

    auto plan = Vector2<float>();
    plan.setX(currentPosition.getX() - target.getX());
    plan.setY(currentPosition.getY() - target.getY());

    float linearVelocity = cos(-heading) * plan.getX() + sin(-heading) * plan.getY();
    float angularVelocity = (sin(-heading) * plan.getX() - cos(-heading) * plan.getY()) / m_ControlPointDistance;
    m_pMotorController->move(linearVelocity, angularVelocity);
    */

    float lineError = m_ReferenceLine - m_Odometry.getU();
    float angularVelocity = m_pSystemController->run(lineError);
    m_pMotorController->move(0.5, angularVelocity);
    return;
  }

  /*
  Se já estamos a uma distância mínima do nosso objetivo (garante que não
  tenham problemas e nunca mude de estado) muda para 'refinedSearch'
  */
  m_CurrentMode = &Trekking::refinedSearch;
}

void Trekking::refinedSearch(unsigned long deltaTime)
{
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
      m_pMotorController->move(0, -1);
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
  m_Map = Matrix<float>(10, 10, 0);

  /*
  Aqui lemos o ultrassom e calculamos a posição que devemos colocar o obstaculo
  no mapa
  */
}

void Trekking::standBy(unsigned long deltaTime)
{
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
  turnBuzzerOn();
  delay(5000);
  turnBuzzerOff();

  m_CurrentTargetId++;
  m_CurrentMode = &Trekking::rotateToTarget;
}

void Trekking::finish(unsigned long deltaTime)
{
  // Chama o buzzer pra evitar reescrever o mesmo código
  buzzer(deltaTime);

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
