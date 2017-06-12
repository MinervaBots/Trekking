#include "LonguinhoEncoder.hpp"
#include <Arduino.h>

void LonguinhoEncoder::update(Position *pPosition)
{
  float radius = m_pMotorController->getWheelsRadius();
  float distanceFromAxis = m_pMotorController->getWheelsDistanceFromRotationAxis();
  float leftVelocity = m_pMotorController->getLeftVelocity();
  float rightVelocity = m_pMotorController->getRightVelocity();

  unsigned long deltaTime = millis() - m_LastUpdateTime;

  m_DeltaDistanceLeft = 2 * PI * radius * leftVelocity * deltaTime;
  m_DeltaDistanceRight = 2 * PI * radius * rightVelocity * deltaTime;

  float deltaEncoder = getDeltaDistance();
  float deltaEncoderLeft = getDeltaDistanceLeft();
  float deltaEncoderRight = getDeltaDistanceRight();

  float heading = pPosition->getHeading();
  float xPrime = deltaEncoder * cos(heading);
  float yPrime = deltaEncoder * sin(heading);
  float headingPrime = (deltaEncoderRight - deltaEncoderLeft) / distanceFromAxis;

  pPosition->setX(pPosition->getX() + xPrime);
  pPosition->setY(pPosition->getY() + yPrime);
  pPosition->setHeading(pPosition->getHeading() + headingPrime);

  m_LastUpdateTime = millis();
}
