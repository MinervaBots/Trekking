#ifndef LonguinhoSensoring_hpp
#define LonguinhoSensoring_hpp

#include "../lib/Filter/SimpleMovingAverageFilter.hpp"
#include "../lib/Trekking/TrekkingSensoring.hpp"
#include "../lib/MPU9150Lib/MPU9150Lib.h"
#include "../lib/InputSource/InputSource.hpp"
#include "../lib/TCS230/TCS230.h"
#include "../lib/Ultrasonic/Ultrasonic.hpp"
#include "../lib/Position/Position.hpp"
#include "LonguinhoMotorController.hpp"
#include "LonguinhoEncoder.hpp"

class LonguinhoSensoring : public TrekkingSensoring
{
public:
  LonguinhoSensoring();
  TrekkingOdometry getInput();

  void intializePosition(float x, float y, float heading)
  {
    m_pCurrentEncoderPosition.setX(x);
    m_pCurrentEncoderPosition.setY(y);
    m_pCurrentEncoderPosition.setHeading(heading);

    m_pCurrentMPUPosition.setX(x);
    m_pCurrentMPUPosition.setY(y);
    m_pCurrentMPUPosition.setHeading(heading);
  }

  void initializeEncoder(LonguinhoMotorController *pMotorController);
  void initializeMPU(int mpuRate, int magMix, int magUpdateRate, int lpfRate);

  Position getEncoderPosition() const { return m_pCurrentEncoderPosition; }
  Position getMPUPosition() const { return m_pCurrentMPUPosition; }

  Position getPosition() const { return getEncoderPosition(); }
private:
  TrekkingOdometry m_CachedValue;

  LonguinhoEncoder m_Encoder;
  Position m_pCurrentEncoderPosition;

  MPU9150Lib m_MPU;
  SimpleMovingAverageFilter<5> m_MagFilter;
  float m_InitialHeading;
  Position m_pCurrentMPUPosition;

  Ultrasonic m_UltrasonicLeft;
  Ultrasonic m_UltrasonicCenter;
  Ultrasonic m_UltrasonicRight;

  unsigned char m_White;
  TCS230 m_ColorSensorLeft;
  TCS230 m_ColorSensorRight;
};

#endif //LonguinhoSensoring_hpp
