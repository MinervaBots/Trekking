#include "LonguinhoEncoder.hpp"
#include <Arduino.h>
#include <stdint.h>

void LonguinhoEncoder::update(Position *pPosition)
{
  float wheelRadius = m_pMotorController->getWheelsRadius();
  float distanceFromAxis = m_pMotorController->getWheelsDistanceFromRotationAxis();

  int deltaEncoderLeft = m_pMotorController->getEncoderLeft(true);
  int deltaEncoderRight = m_pMotorController->getEncoderRight(true);

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
  //headingPrime *= 1.4;

  float newHeading = heading + headingPrime;
  newHeading = atan2(sin(newHeading), cos(newHeading));
  pPosition->setX(pPosition->getX() + xPrime);
  pPosition->setY(pPosition->getY() + yPrime);
  pPosition->setHeading(newHeading);
  //pPosition->setHeading(heading);
/*
  m_LastEncoderLeft = encoderLeft;
  m_LastEncoderRight = encoderRight;
  m_LastRun =
   millis();
*/
}
