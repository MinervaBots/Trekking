#include "LonguinhoEncoder.hpp"
#include <Arduino.h>

void LonguinhoEncoder::update(Position *pPosition)
{
  float wheelRadius = m_pMotorController->getWheelsRadius();
  float distanceFromAxis = m_pMotorController->getWheelsDistanceFromRotationAxis();

  float correctionFactor = m_pMotorController->getPulsesPerRotation() * m_pMotorController->getGearRate();
  m_DeltaDistanceLeft = 2 * PI * wheelRadius * m_pMotorController->getEncoderLeft() / correctionFactor;
  m_DeltaDistanceRight = 2 * PI * wheelRadius * m_pMotorController->getEncoderRight() / correctionFactor;

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
  Serial.print(", ");
  Serial.print(headingPrime);
  Serial.println();
  //delay(250);
  pPosition->setX(pPosition->getX() + xPrime);
  pPosition->setY(pPosition->getY() + yPrime);
  //pPosition->setHeading(pPosition->getHeading() + headingPrime);
}
