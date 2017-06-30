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
  float getPulsesPerRotation() { return m_PulsesPerRotation; }
  float getGearRate() { return m_GearRate; }

  int getEncoderLeft();
  int getEncoderRight();

  void resetEncoders(){ m_RoboClaw.ResetEncoders(m_Address); }

private:
  /*----|RoboClaw|------------------------------------------------------------*/
  unsigned char m_Address;
  RoboClaw m_RoboClaw;

  unsigned char m_StatusLeft, m_StatusRight;
  bool m_ValidLeft, m_ValidRight;
  /*--------------------------------------------------------------------------*/

  /*----|Variáveis de controle|-----------------------------------------------*/
  float m_PulsesPerRotation = 64;
  float m_GearRate = 70;
  float m_MaxPPS = m_GearRate*m_PulsesPerRotation*150/60;
  float m_SafeRPS = m_MaxPPS*0.9;
  /*--------------------------------------------------------------------------*/

  /*----|Pinos|---------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/



  unsigned char mapPWM(float pps);
};

#endif //LonguinhoMotorController_hpp
