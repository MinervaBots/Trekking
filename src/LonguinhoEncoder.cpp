#include "LonguinhoEncoder.hpp"
#include <Arduino.h>

void LonguinhoEncoder::update(Position *pPosition)
{
  float radius = m_pMotorController->getWheelsRadius();
  float distanceFromAxis = m_pMotorController->getWheelsDistanceFromRotationAxis();
  float leftVelocity = m_pMotorController->getLeftVelocity();
  float rightVelocity = m_pMotorController->getRightVelocity();


  float deltaTime = (millis() - m_LastUpdateTime) / 1000;

  m_DeltaDistanceLeft = 2 * PI * radius * m_pMotorController->getEncoderLeft() / m_pMotorController->getPulsesPerRotation();
  m_DeltaDistanceRight = 2 * PI * radius *  m_pMotorController->getEncoderRight() / m_pMotorController->getPulsesPerRotation();

  float deltaEncoder = getDeltaDistance();
  float deltaEncoderLeft = getDeltaDistanceLeft();
  float deltaEncoderRight = getDeltaDistanceRight();


  float heading = pPosition->getHeading();
  float xPrime = deltaEncoder * cos(heading);
  float yPrime = deltaEncoder * sin(heading);
  float headingPrime = (deltaEncoderRight - deltaEncoderLeft) / distanceFromAxis;

  Serial.print("Encoder: ");
  Serial.print(deltaEncoderLeft);
  Serial.print(", ");
  Serial.print(deltaEncoderRight);
  Serial.print(", ");
  Serial.print(xPrime);
  Serial.print(", ");
  Serial.print(yPrime);
  Serial.println();

  pPosition->setX(pPosition->getX() + xPrime);
  pPosition->setY(pPosition->getY() + yPrime);
  pPosition->setHeading(pPosition->getHeading() + headingPrime);

  m_LastUpdateTime = millis();
}
