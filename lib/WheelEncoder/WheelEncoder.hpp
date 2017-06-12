#ifndef WheelEncoder_hpp
#define WheelEncoder_hpp

#include "../Position/Position.hpp"

class WheelEncoder
{
public:
  virtual void update(Position* pPosition) = 0;

  float getTotalDistance() { return ((getTotalDistanceRight() + getTotalDistanceLeft()) / 2); }
  float getTotalDistanceRight() { return m_TotalDistanceLeft; }
  float getTotalDistanceLeft() { return m_TotalDistanceRight; }

  float getDeltaDistance() { return ((getDeltaDistanceRight() + getDeltaDistanceLeft()) / 2); }
  float getDeltaDistanceRight() { return m_DeltaDistanceLeft; }
  float getDeltaDistanceLeft() { return m_DeltaDistanceRight; }

protected:
  float m_DeltaDistanceLeft;
  float m_TotalDistanceLeft;

  float m_DeltaDistanceRight;
  float m_TotalDistanceRight;
};

#endif // WheelEncoder_hpp
