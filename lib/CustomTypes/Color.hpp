#ifndef Color_hpp
#define Color_hpp

class Color
{
public:
  Color()
    : Color(0, 0, 0, 0) { }
  Color(unsigned char red, unsigned char green, unsigned char blue)
    : Color(red, green, blue, 0) { }
  Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
    : m_Red(red), m_Green(green), m_Blue(blue), m_Alpha(alpha) { }

  unsigned char getRed() { return m_Red; }
  unsigned char getGreen() { return m_Green; }
  unsigned char getBlue() { return m_Blue; }
  unsigned char getAlpha() { return m_Alpha; }

  void setRed(unsigned char red) { m_Red = red; }
  void setGreen(unsigned char green) { m_Green = green; }
  void setBlue(unsigned char blue) { m_Blue = blue; }
  void setAlpha(unsigned char alpha) { m_Alpha = alpha; }

private:
  unsigned char m_Red;
  unsigned char m_Green;
  unsigned char m_Blue;
  unsigned char m_Alpha;
};

#endif //Color_hpp
