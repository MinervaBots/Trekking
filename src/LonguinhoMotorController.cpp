#include "LonguinhoMotorController.hpp"
#include <SoftwareSerial.h>
#include "../lib/MathHelper/MathHelper.h"
#include "Pins.h"

LonguinhoMotorController::LonguinhoMotorController() :
    m_Address(ROBOCLAW_ADDRESS),
    m_RoboClaw(RX_MOTOR_PIN, TX_MOTOR_PIN, ROBOCLAW_TIMEOUT)
{
  m_RoboClaw.begin(38400);
  //m_RoboClaw.ResetEncoders(m_Address);
  /*
  m_RoboClaw.SetEncM1(m_Address, 0);
  m_RoboClaw.SetEncM2(m_Address, 0);
  */
}

float LonguinhoMotorController::getLeftVelocity()
{
  int32_t pps = m_RoboClaw.ReadSpeedM2(m_Address, &m_StatusLeft, &m_ValidLeft);
  m_LeftVelocity = pps / (m_PulsesPerRotation * m_GearRate);
  return m_LeftVelocity;
}

float LonguinhoMotorController::getRightVelocity()
{
  int32_t pps = m_RoboClaw.ReadSpeedM1(m_Address, &m_StatusRight, &m_ValidRight);
  m_RightVelocity = pps / (m_PulsesPerRotation * m_GearRate);
  return m_RightVelocity;
}

void LonguinhoMotorController::move(float linearVelocity, float angularVelocity)
{
  /*----[Cálculo de Rotações por Segundo (RPS)]---------------------------------
  Equações descritas na página 54~55 da tese do Fraga
  */
	float leftRotation = (linearVelocity + angularVelocity * m_WheelsDistanceFromRotationAxis) /
                                    (2 * PI * m_WheelsRadius);
	float rightRotation = (linearVelocity - angularVelocity * m_WheelsDistanceFromRotationAxis) /
                                    (2 * PI * m_WheelsRadius);

  /*--------------------------------------------------------------------------*/


  /*----[Saturação Diferencial]-------------------------------------------------
  Algoritmo descrito na página 60 da tese do Fraga.
  */
	float rightRotationLimited, leftRotationLimited = 0;

	if (leftRotation < 0 && rightRotation > 0)
  {
		leftRotationLimited = max(leftRotation, -m_SafeRPS); // L negativo
		rightRotationLimited = min(rightRotation, m_SafeRPS); // R positivo
	}
	else if (leftRotation > 0 && rightRotation < 0)
  {
		rightRotationLimited = max(rightRotation, -m_SafeRPS); // R negativo
		leftRotationLimited = min(leftRotation, m_SafeRPS);   // L positivo
	}
	else if (leftRotation > m_SafeRPS || rightRotation > m_SafeRPS)
  {
		if (leftRotation > rightRotation)
    {
			rightRotationLimited = m_SafeRPS * (1 - rightRotation/leftRotation);
			leftRotationLimited = m_SafeRPS;
		}
		else if (leftRotation < rightRotation)
    {
			leftRotationLimited = m_SafeRPS * (1 - leftRotation/rightRotation);
			rightRotationLimited = m_SafeRPS;
		}
		else if (leftRotation == rightRotation)
    {
			rightRotationLimited = m_SafeRPS;
			leftRotationLimited = m_SafeRPS;
		}
	}
	else if (leftRotation < -m_SafeRPS || rightRotation < -m_SafeRPS)
  {
  	if (leftRotation < rightRotation)
    {
    	rightRotationLimited = -m_SafeRPS * (1 - rightRotation/leftRotation);
      leftRotationLimited = -m_SafeRPS;
    }
    else if (leftRotation > rightRotation)
    {
        leftRotationLimited = -m_SafeRPS * (1 - leftRotation/rightRotation);
        rightRotationLimited = -m_SafeRPS;
    }
    else if (leftRotation == rightRotation)
    {
        rightRotationLimited = -m_SafeRPS;
        leftRotationLimited = -m_SafeRPS;
    }
	}
	else
  {
		rightRotationLimited = rightRotation;
		leftRotationLimited = leftRotation;
	}
  /*--------------------------------------------------------------------------*/

  /*
	if(is_testing_search || is_testing_refinedSearch){
		log << "<DESEJADO>";
		log << DELIMITER << v;
		log << DELIMITER << w;
		log << DELIMITER << leftRotation;
		log << DELIMITER << rightRotation;
		log << DELIMITER << leftRotationLimited;
		log << DELIMITER << rightRotationLimited;
	}
  */
	float rightQpps = rightRotationLimited * m_GearRate * m_PulsesPerRotation;
	float leftQpps = leftRotationLimited * m_GearRate * m_PulsesPerRotation;

  m_RoboClaw.ForwardBackwardM1(m_Address, mapPWM(rightQpps));
  m_RoboClaw.ForwardBackwardM2(m_Address, mapPWM(leftQpps));
}

unsigned char LonguinhoMotorController::mapPWM(float pps)
{
  unsigned char pwm = 0;
  float speed = pps / m_MaxPPS;
  pwm = 64 * (1 + speed);
  pwm = max(min(128, pwm), 0);
  return pwm;
}


int LonguinhoMotorController::getEncoderLeft()
{
  int enc = m_RoboClaw.ReadEncM2(m_Address, &m_StatusLeft, &m_ValidLeft);
  m_RoboClaw.SetEncM2(m_Address, 0);
  return enc;
}

int LonguinhoMotorController::getEncoderRight()
{
  int enc = m_RoboClaw.ReadEncM1(m_Address, &m_StatusRight, &m_ValidRight);
  m_RoboClaw.SetEncM1(m_Address, 0);
  return enc;
}
