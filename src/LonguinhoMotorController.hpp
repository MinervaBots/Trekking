#ifndef LonguinhoMotorController_hpp
#define LonguinhoMotorController_hpp

#include "../lib/MotorController/DifferentialDriveController.hpp"
#include "../lib/RoboClaw/RoboClaw.h"

class LonguinhoMotorController : public DifferentialDriveController
{
public:
  LonguinhoMotorController();

  void move(float linearVelocity, float angularVelocity);
  void stop();

  float movingTime(float initialX, float initialY, float finalX, float finalY, float linearVelocity);
  float getLeftVelocityRPS();
  float getRightVelocityRPS();

  float getPulsesPerRotation() { return m_PulsesPerRotation; }
  float getGearRate() { return m_GearRate; }

  int getEncoderLeft(bool reset);
  int getEncoderRight(bool reset);

  void reset(){ m_RoboClaw.ResetEncoders(m_Address); }

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
  float m_MaxRPM = 150;         //Rotations per minute
  float m_MaxRPS = m_MaxRPM/60; //Rotations per second
  float m_SafetyFactor = 0.9;
  float m_MaxPPS = m_GearRate*m_PulsesPerRotation*m_MaxRPS; //Pulses per second
  float m_SafeRPS = m_MaxRPS*m_SafetyFactor; //Safe RPS to be used on calculations
  /*--------------------------------------------------------------------------*/

  /*----|Pinos|---------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/



  unsigned char mapPWM(float pps);
};

#endif //LonguinhoMotorController_hpp
