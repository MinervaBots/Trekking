#ifndef Position_hpp
#define Position_hpp

#include "../Algebra/Vector2.hpp"

class Position : public Vector2<float>
{
public:
  Position() : Vector2<float>(0, 0), m_Heading(0) { }

  float getHeading() const
  {
    return m_Heading;
  }

  void setHeading(float heading)
  {
    m_Heading = heading;
    //m_Heading = atan2(sin(heading), cos(heading));
  }


private:
  float m_Heading;
};

#endif //Position_hpp
