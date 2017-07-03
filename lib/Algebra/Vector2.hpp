#ifndef Vector2_hpp
#define Vector2_hpp

#include <Arduino.h>

template<typename T>
class Vector2
{
public:
  Vector2() {}

  Vector2(T x, T y) : m_X(x), m_Y(y) {}
  T getX() const { return m_X; }
  T getY() const { return m_Y; }
  void setX(T x) { m_X = x; }
  void setY(T y) { m_Y = y; }
/*
  static float euclideanDistance(Vector2<T> vector1, Vector2<T> vector2)
  {
    return sqrt(pow(vector1.m_X - vector2.m_X, 2) + pow(vector1.m_Y - vector2.m_Y, 2));
  }
*/
  Vector2<T> operator+(const Vector2<T>& vector) const
  {
    Vector2<T> ret;
    ret.m_X = this->m_X + vector.m_X;
    ret.m_Y = this->m_Y + vector.m_Y;
    return ret;
  }
  Vector2<T> operator-(const Vector2<T>& vector) const
  {
    Vector2<T> ret;
    ret.m_X = this->m_X - vector.m_X;
    ret.m_Y = this->m_Y - vector.m_Y;
    return ret;
  }
  Vector2<T>& operator+=(const Vector2<T>& vector)
  {
    this->m_X += vector.m_X;
    this->m_Y += vector.m_Y;
    return *this;
  }
  Vector2<T>& operator-=(const Vector2<T>& vector)
  {
    this->m_X -= vector.m_X;
    this->m_Y -= vector.m_Y;
    return *this;
  }
  Vector2<T>& operator/=(const T& scalar)
  {
    this->m_X /= scalar;
    this->m_Y /= scalar;
    return *this;
  }
  Vector2<T>& operator*=(const T& scalar)
  {
    this->m_X *= scalar;
    this->m_Y *= scalar;
    return *this;
  }
  bool operator==(const Vector2<T>& vector) const
  {
    return this->m_X == vector.m_X && this->m_Y == vector.m_Y;
  }

private:
  T m_X;
  T m_Y;
};

#endif //Vector2_hpp
