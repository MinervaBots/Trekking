#include "LonguinhoEncoder.hpp"
#include <Arduino.h>

void LonguinhoEncoder::update(Position *pPosition)
{
  float wheelRadius = m_pMotorController->getWheelsRadius();
  float distanceFromAxis = m_pMotorController->getWheelsDistanceFromRotationAxis();

  long deltaEncoderLeft = m_pMotorController->getEncoderLeft() - m_LastEncoderLeft;
  long deltaEncoderRight = m_pMotorController->getEncoderRight() - m_LastEncoderRight;

  float correctionFactor = m_pMotorController->getPulsesPerRotation() * m_pMotorController->getGearRate();
  m_DeltaDistanceLeft = 2 * PI * wheelRadius * deltaEncoderLeft / correctionFactor;
  m_DeltaDistanceRight = 2 * PI * wheelRadius * deltaEncoderRight / correctionFactor;

  float deltaEncoderDistance = getDeltaDistance();
  float deltaEncoderLeftDistance = getDeltaDistanceLeft();
  float deltaEncoderRightDistance = getDeltaDistanceRight();

  float heading = pPosition->getHeading();
  float xPrime = deltaEncoderDistance * cos(heading);
  float yPrime = deltaEncoderDistance * sin(heading);
  float headingPrime = (deltaEncoderRightDistance - deltaEncoderLeftDistance) / (2 * distanceFromAxis);
  /*
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
  //*/
  float newHeading = pPosition->getHeading() + headingPrime;
  newHeading = atan2(sin(newHeading), cos(newHeading));
  pPosition->setX(pPosition->getX() + xPrime);
  pPosition->setY(pPosition->getY() + yPrime);
  pPosition->setHeading(newHeading);

  m_LastEncoderLeft = m_pMotorController->getEncoderLeft();
  m_LastEncoderRight = m_pMotorController->getEncoderRight();
}
