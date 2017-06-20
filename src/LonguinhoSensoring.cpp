#include "LonguinhoSensoring.hpp"
#include "Pins.h"

LonguinhoSensoring::LonguinhoSensoring() :
  m_MagFilter(5),
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
    float dir = atan2(m_MPU.m_calMag[VEC3_Y], m_MPU.m_calMag[VEC3_X]);
    dir = m_MagFilter.getInput(dir);

    Serial.print("Magnetometro: ");
    Serial.println(dir);
    
    m_CachedValue.setU(dir);
    m_pCurrentMPUPosition.setHeading(m_MPU.m_dmpEulerPose[VEC3_Z] - m_InitialHeading);
  }

  auto position = getMPUPosition();
  Serial.print("MPU: ");
  Serial.print(position.getX());
  Serial.print(" ");
  Serial.print(position.getY());
  Serial.print(" ");
  Serial.println(position.getHeading());

  position = getEncoderPosition();
  Serial.print("Encoder: ");
  Serial.print(position.getX());
  Serial.print(" ");
  Serial.print(position.getY());
  Serial.print(" ");
  Serial.println(position.getHeading());

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
