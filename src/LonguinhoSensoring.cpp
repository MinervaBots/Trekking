#include "LonguinhoSensoring.hpp"
#include "Pins.h"
#include "../lib/Logger/Logger.hpp"

LonguinhoSensoring::LonguinhoSensoring() :
  m_UltrasonicLeft(LEFT_SONAR_RX_PIN, LEFT_SONAR_TX_PIN, 30, 200),
  m_UltrasonicCenter(CENTER_SONAR_RX_PIN, RIGHT_SONAR_TX_PIN, 30, 200),
  m_UltrasonicRight(RIGHT_SONAR_RX_PIN, RIGHT_SONAR_TX_PIN, 30, 200),
  m_ColorSensorLeft(LEFT_COLOR_S0,LEFT_COLOR_S1,LEFT_COLOR_S2,LEFT_COLOR_S3,LEFT_COLOR_OUTPUT,WHITE_VALUE),
  m_ColorSensorRight(RIGHT_COLOR_S0,RIGHT_COLOR_S1,RIGHT_COLOR_S2,RIGHT_COLOR_S3,RIGHT_COLOR_OUTPUT,WHITE_VALUE)
{

}

void LonguinhoSensoring::initializeEncoder(LonguinhoMotorController *pMotorController)
{
  m_Encoder.initialize(pMotorController);
}

void LonguinhoSensoring::initializeMPU(int mpuRate, int magMix, int magUpdateRate, int lpfRate)
{
  m_MPU.useAccelCal(true);
  m_MPU.useMagCal(true);

  m_MPU.init(mpuRate, magMix,	magUpdateRate, lpfRate);
  delay(2000);
  //*

  bool inited = false;
  while(!inited)
  {
    inited = m_MPU.read();
    m_InitialHeading = m_MPU.m_dmpEulerPose[2];
    Serial.println(m_InitialHeading);
  }
  //*/
}

TrekkingOdometry LonguinhoSensoring::getInput()
{
  m_Encoder.update(&m_pCurrentEncoderPosition);
  /*
  if(m_ColorSensorLeft.getInput().getAlpha() >= m_White &&
    m_ColorSensorRight.getInput().getAlpha() >= m_White)
  {
    m_CachedValue.setT(true);
  }
  else
  {
    m_CachedValue.setT(false);
  }
  */

  if(m_MPU.read())
  {
    float magDirection = atan2(m_MPU.m_calMag[VEC3_Y], m_MPU.m_calMag[VEC3_X]);
    if(m_pMagFilter != nullptr)
    {
      magDirection = m_pMagFilter->getInput(magDirection);
    }

    Log->debug("Magnetometro: %f", magDirection);

    m_CachedValue.setU(magDirection);


    m_CurrentVelocity += Vector2<float>(m_MPU.m_calAccel[VEC3_X], m_MPU.m_calAccel[VEC3_Y]);

    // [TODO] - Verificar isso
    auto xAlignedVel = m_CurrentVelocity.getX() * sin(m_pCurrentMPUPosition.getHeading());
    auto yAlignedVel = m_CurrentVelocity.getY() * cos(m_pCurrentMPUPosition.getHeading());
    
    m_pCurrentMPUPosition += Vector2<float>(xAlignedVel, yAlignedVel);
    m_pCurrentMPUPosition.setHeading(m_MPU.m_fusedEulerPose[VEC3_Z] - m_InitialHeading);
  }

  auto position = getMPUPosition();
  Log->debug("MPU Position: %f, %f, %f", position.getX(), position.getY(), position.getHeading());

  position = getEncoderPosition();
  Log->debug("Encoder Position: %f, %f, %f", position.getX(), position.getY(), position.getHeading());

  /*
  Avaliar os sensores de ultrassom. O vetor resultado disse deve indicar a
  direção e distância até o que for detectado
  */
  /*
  auto leftDistance = m_UltrasonicLeft.getInput();
  auto centerDistance = m_UltrasonicCenter.getInput();
  auto rightDistance = m_UltrasonicRight.getInput();

  float x = 0;
  float y = 0;
  unsigned char cnt;
  if(leftDistance > 0)
  {
    x -= 15;
    y += leftDistance;
    cnt++;
  }
  if(rightDistance > 0)
  {
    x += 15;
    y += rightDistance;
    cnt++;
  }
  if(centerDistance > 0)
  {
    y += centerDistance;
    cnt++;
  }

  if(cnt < 0)
  {
    x = -1;
    y = -1;
  }
  else
  {
    y /= cnt;
    x /= cnt;
    // Somei em graus alí em cima, agora converte pra radianos
    x = radians(x);
  }

  Vector2<float> result(x, y);
  m_CachedValue.setV(result);
  */
  return m_CachedValue;
}
