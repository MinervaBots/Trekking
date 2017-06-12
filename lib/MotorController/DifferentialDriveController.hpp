#ifndef DifferentialDriveController_hpp
#define DifferentialDriveController_hpp

#include "MotorController.hpp"

class DifferentialDriveController : public MotorController
{
public:
  DifferentialDriveController()
      : DifferentialDriveController(0, 0) {}
  DifferentialDriveController(float wheelsRadius, float distanceFromRotationAxis);
  virtual void move(float linearVelocity, float angularVelocity);

  float getWheelsRadius() { return m_WheelsRadius; }
  float getWheelsDistanceFromRotationAxis() { return m_WheelsDistanceFromRotationAxis; }

  virtual float getLeftVelocity() { return m_LeftVelocity; }
  virtual float getRightVelocity() { return m_RightVelocity; }


  void setWheelsRadius(float wheelsRadius);
  void setWheelsDistanceFromRotationAxis(float wheelsDistanceFromRotationAxis);

protected:
  float m_LeftVelocity;
  float m_RightVelocity;
  float m_WheelsRadius;
  float m_WheelsDistanceFromRotationAxis;
};

#endif //DifferentialDriveController_hpp
