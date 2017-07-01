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

//Returns the number of rotations that the wheel does on a second
float LonguinhoMotorController::getLeftVelocityRPS()
{
  int32_t pps = m_RoboClaw.ReadSpeedM2(m_Address, &m_StatusLeft, &m_ValidLeft);
  m_LeftVelocity = pps / (m_PulsesPerRotation * m_GearRate);
  return m_LeftVelocity;
}
// Returns the number of rotations that the wheel does on a segond
float LonguinhoMotorController::getRightVelocityRPS()
{
  int32_t pps = m_RoboClaw.ReadSpeedM1(m_Address, &m_StatusRight, &m_ValidRight);
  m_RightVelocity = pps / (m_PulsesPerRotation * m_GearRate);
  return m_RightVelocity;
}

void LonguinhoMotorController::move(float linearVelocity, float angularVelocity)
{
  //DifferentialDriveController::move(linearVelocity, angularVelocity);
  /*----[Cálculo de Rotações por Segundo (RPS)]---------------------------------
  Equações descritas na página 54~55 da tese do Fraga
  */
	float leftWheelRotation = (linearVelocity + angularVelocity * m_WheelsDistanceFromRotationAxis) /
                                    (2 * PI * m_WheelsRadius);
	float rightWheelRotation = (linearVelocity - angularVelocity * m_WheelsDistanceFromRotationAxis) /
                                    (2 * PI * m_WheelsRadius);
  /*--------------------------------------------------------------------------*/


  /*----[Saturação Diferencial]-------------------------------------------------
  Algoritmo descrito na página 60 da tese do Fraga.
  I don't know for sure what it does, but works just fine.
  */
	float rightWheelRotationLimited = 0;
  float leftWheelRotationLimited = 0;

	if (leftWheelRotation < 0 && rightWheelRotation > 0)
  {
		leftWheelRotationLimited = max(leftWheelRotation, -m_SafeRPS); // L negativo
		rightWheelRotationLimited = min(rightWheelRotation, m_SafeRPS); // R positivo
	}
	else if (leftWheelRotation > 0 && rightWheelRotation < 0)
  {
		rightWheelRotationLimited = max(rightWheelRotation, -m_SafeRPS); // R negativo
		leftWheelRotationLimited = min(leftWheelRotation, m_SafeRPS);   // L positivo
	}
	else if (leftWheelRotation > m_SafeRPS || rightWheelRotation > m_SafeRPS)
  {
		if (leftWheelRotation > rightWheelRotation)
    {
			rightWheelRotationLimited = m_SafeRPS * (1 - rightWheelRotation/leftWheelRotation);
			leftWheelRotationLimited = m_SafeRPS;
		}
		else if (leftWheelRotation < rightWheelRotation)
    {
			leftWheelRotationLimited = m_SafeRPS * (1 - leftWheelRotation/rightWheelRotation);
			rightWheelRotationLimited = m_SafeRPS;
		}
		else if (leftWheelRotation == rightWheelRotation)
    {
			rightWheelRotationLimited = m_SafeRPS;
			leftWheelRotationLimited = m_SafeRPS;
		}
	}
	else if (leftWheelRotation < -m_SafeRPS || rightWheelRotation < -m_SafeRPS)
  {
  	if (leftWheelRotation < rightWheelRotation)
    {
    	rightWheelRotationLimited = -m_SafeRPS * (1 - rightWheelRotation/leftWheelRotation);
      leftWheelRotationLimited = -m_SafeRPS;
    }
    else if (leftWheelRotation > rightWheelRotation)
    {
        leftWheelRotationLimited = -m_SafeRPS * (1 - leftWheelRotation/rightWheelRotation);
        rightWheelRotationLimited = -m_SafeRPS;
    }
    else if (leftWheelRotation == rightWheelRotation)
    {
        rightWheelRotationLimited = -m_SafeRPS;
        leftWheelRotationLimited = -m_SafeRPS;
    }
	}
	else
  {
		rightWheelRotationLimited = rightWheelRotation;
		leftWheelRotationLimited = leftWheelRotation;
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
	float rightQpps = rightWheelRotationLimited * m_GearRate * m_PulsesPerRotation;
	float leftQpps = leftWheelRotationLimited * m_GearRate * m_PulsesPerRotation;

  m_RoboClaw.ForwardBackwardM1(m_Address, mapPWM(rightQpps));
  m_RoboClaw.ForwardBackwardM2(m_Address, mapPWM(leftQpps));
}


//Faz com que os dois motores se movam com uma mesma velocidade constante
float LonguinhoMotorController::movingTime(float initialX, float initialY, float finalX, float finalY, float linearVelocity)
{
  float distance = sqrt(pow(finalX - initialX, 2) + pow(finalY - initialY, 2));
  return linearVelocity/distance;
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
  //m_RoboClaw.SetEncM2(m_Address, 0);
  return enc;
}

int LonguinhoMotorController::getEncoderRight()
{
  int enc = m_RoboClaw.ReadEncM1(m_Address, &m_StatusRight, &m_ValidRight);
  //m_RoboClaw.SetEncM1(m_Address, 0);
  return enc;
}
