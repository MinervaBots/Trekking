#ifndef Tuple_hpp
#define Tuple_hpp

template <typename T, typename U, typename V>
class Tuple
{
public:
  void setT(T valueT) { m_ValueT = valueT; }
  void setU(U valueU) { m_ValueU = valueU; }
  void setV(V valueV) { m_ValueV = valueV; }

  T getT() { return m_ValueT; }
  U getU() { return m_ValueU; }
  V getV() { return m_ValueV; }

private:
  T m_ValueT;
  U m_ValueU;
  V m_ValueV;
};

#endif
