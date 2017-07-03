#include "LonguinhoEncoder.hpp"
#include <Arduino.h>

void LonguinhoEncoder::update(Position *pPosition)
{
  float deltaTimeInSeconds = (millis() - m_LastRun) / 1000.0;

  float wheelRadius = m_pMotorController->getWheelsRadius();
  float distanceFromAxis = m_pMotorController->getWheelsDistanceFromRotationAxis();

  float leftVelocityRPS = m_pMotorController->getLeftVelocityRPS();
  float rightVelocityRPS = m_pMotorController->getRightVelocityRPS();

  m_DeltaDistanceLeft = 2 * PI * wheelRadius * leftVelocityRPS * deltaTimeInSeconds;
  m_DeltaDistanceLeft *= 0.98;

  m_DeltaDistanceRight = 2 * PI * wheelRadius * rightVelocityRPS * deltaTimeInSeconds;

  float deltaEncoderDistance = getDeltaDistance();
  float deltaEncoderLeftDistance = getDeltaDistanceLeft();
  float deltaEncoderRightDistance = getDeltaDistanceRight();

  float heading = pPosition->getHeading();
  float xPrime = deltaEncoderDistance * cos(heading);
  float yPrime = deltaEncoderDistance * sin(heading);
  float headingPrime = (deltaEncoderRightDistance - deltaEncoderLeftDistance) / (2 * distanceFromAxis);

  float newHeading = pPosition->getHeading() + headingPrime;
  newHeading = atan2(sin(newHeading), cos(newHeading));
  pPosition->setX(pPosition->getX() + xPrime);
  pPosition->setY(pPosition->getY() + yPrime);
  pPosition->setHeading(newHeading);

  m_LastRun = millis();
}

/*
void LonguinhoEncoder::update(Position *pPosition)
{
  float deltaTimeInSeconds = (millis() - m_LastRun) / 1000.0;
  float wheelRadius = m_pMotorController->getWheelsRadius();
  float distanceFromAxis = m_pMotorController->getWheelsDistanceFromRotationAxis();

  int encoderLeft = m_pMotorController->getEncoderLeft();
  int encoderRight = m_pMotorController->getEncoderRight();

  int deltaEncoderLeft = encoderLeft - m_LastEncoderLeft;
  int deltaEncoderRight = encoderRight - m_LastEncoderRight;

  float correctionFactor = m_pMotorController->getPulsesPerRotation() * m_pMotorController->getGearRate();
  if(deltaEncoderLeft > -30000)
  {
    m_DeltaDistanceLeft = 2 * PI * wheelRadius * deltaEncoderLeft / correctionFactor;
  }
  else
  {
    m_DeltaDistanceLeft = 2 * PI * wheelRadius * m_pMotorController->getLeftVelocityRPS() * deltaTimeInSeconds;
    m_DeltaDistanceLeft *= 0.98;
  }

  if(deltaEncoderRight > -30000)
  {
    m_DeltaDistanceRight = 2 * PI * wheelRadius * deltaEncoderRight / correctionFactor;
  }
  else
  {
    m_DeltaDistanceRight = 2 * PI * wheelRadius * m_pMotorController->getRightVelocityRPS() * deltaTimeInSeconds;
  }


  float deltaEncoderDistance = getDeltaDistance();
  float deltaEncoderLeftDistance = getDeltaDistanceLeft();
  float deltaEncoderRightDistance = getDeltaDistanceRight();

  float heading = pPosition->getHeading();
  float xPrime = deltaEncoderDistance * cos(heading);
  float yPrime = deltaEncoderDistance * sin(heading);
  float headingPrime = (deltaEncoderRightDistance - deltaEncoderLeftDistance) / (2 * distanceFromAxis);

  float newHeading = pPosition->getHeading() + headingPrime;
  newHeading = atan2(sin(newHeading), cos(newHeading));
  pPosition->setX(pPosition->getX() + xPrime);
  pPosition->setY(pPosition->getY() + yPrime);
  pPosition->setHeading(newHeading);

  m_LastEncoderLeft = encoderLeft;
  m_LastEncoderRight = encoderRight;
  m_LastRun = millis();
}
*/
