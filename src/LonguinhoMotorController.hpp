#ifndef LonguinhoMotorController_hpp
#define LonguinhoMotorController_hpp

#include "../lib/MotorController/DifferentialDriveController.hpp"
#include "../lib/RoboClaw/RoboClaw.h"

class LonguinhoMotorController : public DifferentialDriveController
{
public:
  LonguinhoMotorController();
  void move(float linearVelocity, float angularVelocity);

  float getLeftVelocity();
  float getRightVelocity();

private:
  /*----|RoboClaw|------------------------------------------------------------*/
  unsigned char m_Address;
  SoftwareSerial m_SoftwareSerial;
  RoboClaw m_RoboClaw;

  unsigned char m_StatusRight, m_StatusLeft;
  bool m_ValidRight, m_ValidLeft;
  /*--------------------------------------------------------------------------*/

  /*----|Variáveis de controle|-----------------------------------------------*/
  float m_PulsesPerRotation;
  float m_SafeRPS;
  float m_GearRate;
  float m_MaxPPS;
  /*--------------------------------------------------------------------------*/

  /*----|Pinos|---------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/



  unsigned char mapPWM(float pps);
};

#endif //LonguinhoMotorController_hpp
