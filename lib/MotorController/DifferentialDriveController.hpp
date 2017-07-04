#ifndef DifferentialDriveController_hpp
#define DifferentialDriveController_hpp

class DifferentialDriveController
{
public:
  DifferentialDriveController() { }
  DifferentialDriveController(float wheelsRadius, float distanceFromRotationAxis);
  virtual void move(float linearVelocity, float angularVelocity);
  virtual void stop() = 0;
  virtual void reset() = 0;
  
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
