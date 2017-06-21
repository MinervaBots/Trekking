#ifndef Trekking_hpp
#define Trekking_hpp

#include "TrekkingSensoring.hpp"
#include "../Position/Position.hpp"
#include "../Pathfinding/AStar.hpp"
#include "../Collections/LinkedList.h"
#include "../SystemController/SystemController.hpp"
#include "../MotorController/DifferentialDriveController.hpp"


class Trekking
{
public:
  void addTarget(unsigned char x, unsigned char y)
  {
    m_Targets.add(Vector2<unsigned char>(x, y));
  }
  void setup();
  void start();
  void stop();
  void update();
  void emergency();



  /*----|Getters e setters|---------------------------------------------------*/
  bool isRunning() { return m_IsRunning; }
  void setOperationMode(bool automatic) { m_OperationMode = automatic; }

  void setBuzzerPin(unsigned char pin) { m_BuzzerPin = pin; }

  void setMotorController(DifferentialDriveController *pMotorController, float controlPointDistance)
  {
    m_pMotorController = pMotorController;
    m_ControlPointDistance = controlPointDistance;
  }

  void setSystemController(SystemController *controller) { m_pSystemController = controller; }
  void setSensoring(TrekkingSensoring *sensoring) { m_pTrekkingSensoring = sensoring; }

  void setMaxTimeInRefinedSearch(unsigned long maxTime) { m_MaxTimeInRefinedSearch = maxTime; }
  /*--------------------------------------------------------------------------*/

private:
  /*----|Variáveis gerais|----------------------------------------------------*/
  bool m_IsRunning;
  bool m_OperationMode;
  unsigned long m_StartTime;
  unsigned long m_StopTime;
  unsigned long m_LastIterationTime;
  /*--------------------------------------------------------------------------*/


  /*----|Pinos|---------------------------------------------------------------*/
  unsigned char m_BuzzerPin;
  /*--------------------------------------------------------------------------*/


  /*----|Variáveis de controle|-----------------------------------------------*/
  float m_ControlPointDistance;
  SystemController *m_pSystemController;
  DifferentialDriveController *m_pMotorController;
  /*--------------------------------------------------------------------------*/


  /*----|Variáveis de navegação|----------------------------------------------*/
  AStar m_Pathfinder;
  Matrix<float>* m_pMap;
  unsigned char m_CurrentTargetId;
  LinkedList<Vector2<unsigned char>> m_Targets;
  TrekkingSensoring *m_pTrekkingSensoring;
  TrekkingOdometry m_Odometry;
  float m_ReferenceLine;
  unsigned long m_StartTimeInRefinedSearch;
  unsigned long m_MaxTimeInRefinedSearch;
  /*--------------------------------------------------------------------------*/

  /*----|Modos de operação|---------------------------------------------------*/
  void (Trekking::*m_CurrentMode)(unsigned long);

  /*
  Aguarda pressionar o botão de inicio.
  [Próximo estado]: search
  */
	void standBy(unsigned long deltaTime);

  /*
  Gira na direção do alvo que queremos alcançar no momento.
  [Próximo estado]: search
  */
  void rotateToTarget(unsigned long deltaTime);

  /*
  Segue com PID uma linha imaginária que vem do magnetometro até chegar que a
  posição seja igual a do alvo pela odometria. Quando estiver suficientemente
  próximo (< 3m) do objetivo, utiliza os sensores de ultrassom tentando
  localizar o alvo com maior rapidez.

  [Proximo estado]: refinedSearch
  [Proximo estado]: buzzer
  [Proximo estado]: avoidObstacles
  */
  void search(unsigned long deltaTime);

  /*
  Usa os sensores de ultrassom para localizar o cone e valida isso com a detecção
  da cor branca pelos sensores RGB em baixo do corpo.
  Com o alvo alcançado, reseta a posição (sem alterar a rotação) para a do alvo
  e ativa a sirene.
  [Proximo estado]: buzzer

  Caso o alvo seja o terceiro ponto, e algo seja detectado com uma distância
  inferior a 70 cm, e a cor na parte de baixo não seja branca, trata como
  obstáculo.
  [Proximo estado]: avoidObstacles
  */
  void refinedSearch(unsigned long deltaTime);

  /*
  Ativa a sirene e inicia um delay por 5 segundos e então a sirene é desativada.
  [Proximo estado]: rotateToTarget
  */
	void buzzer(unsigned long deltaTime);

  /*
  Utiliza o algoritmo de pathfinding A* para manobrar através dos obstáculos até
  alcançar o ultimo objetivo.
  [Proximo estado]: finish
  */
  void avoidObstacles(unsigned long deltaTime);

  /*
  Sinaliza que o objetivo foi alcançado e para.
  [Proximo estado]: standBy
  */
  void finish(unsigned long deltaTime);
  /*--------------------------------------------------------------------------*/


  /*----|Funções operacionais|------------------------------------------------*/
  void turnBuzzerOn();
	void turnBuzzerOff();

  float distance(Vector2<unsigned char> vector)
  {
    auto currentPosition = m_pTrekkingSensoring->getPosition();
    return sqrt(
      pow(currentPosition.getX() - vector.getX(), 2) +
      pow(currentPosition.getY() - vector.getY(), 2)
    );
  }
  /*--------------------------------------------------------------------------*/
};

#endif //Trekking_hpp
