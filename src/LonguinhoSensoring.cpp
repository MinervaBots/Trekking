#include "LonguinhoSensoring.hpp"
#include "Pins.h"

LonguinhoSensoring::LonguinhoSensoring() :
  imu(&settings),
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
  Serial.print("ArduinoIMU starting using device ");

  int errcode;
  Serial.println(imu.IMUName());
  if ((errcode = imu.IMUInit()) < 0)
  {
    Serial.print("Failed to init IMU: ");
    Serial.println(errcode);
  }

  if (imu.getCalibrationValid())
    Serial.println("Using compass calibration");
  else
    Serial.println("No valid compass calibration data");
  delay(1000);

  while (true)
  {
    if(imu.IMURead())
    {
      //m_InitialHeading = hdm;
      //Serial.print("Initial Heading: ");
      //Serial.println(m_InitialHeading);
      break;
    }
  }
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
  while (true)
  {
    if(imu.IMURead())
    {
      RTVector3 mag = imu.getCompass();
      float magDirection = atan2(mag.y(), mag.x()); //- m_InitialHeading;
      if(m_pMagFilter != nullptr)
      {
        magDirection = m_pMagFilter->getInput(magDirection);
      }
      //Serial.print("Magnetometro: ");
      //Serial.println(magDirection);

      m_CachedValue.setU(magDirection);
      //m_pCurrentEncoderPosition.setHeading(magDirection);
      break;
    }
  }
/*
  auto position = getMPUPosition();
  Log.debug("MPU Position X", position.getX());
  Log.debug("MPU Position Y", position.getY());
  Log.debug("MPU Position Heading", position.getHeading());
*/
  auto position = m_pCurrentEncoderPosition;
  Serial.print("X: ");
  Serial.print(position.getX());
  Serial.print(", ");
  Serial.print("Y: ");
  Serial.print(position.getY());
  Serial.print(", ");
  Serial.print("Heading: ");
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
