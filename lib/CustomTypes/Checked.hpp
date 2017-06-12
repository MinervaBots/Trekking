#ifndef Checked_hpp
#define Checked_hpp

#include <Arduino.h>
#include <assert.h>

template<typename T>
class Checked
{
public:
  Checked(bool isValid) : m_IsValid(isValid) {}
  Checked(bool isValid, T value) : m_IsValid(isValid), m_Value(value) {}

  bool IsValid() { return m_IsValid; }
  T getValue() const { return m_Value; }

  operator T(void) const
  {
    /*
    O ideal seria lançar uam exceção que pudessemos tratar.
    Mas o Arduino não suporta isso, então fazemos um assert que aborta o programa se falhar.
    */
    assert(m_IsValid == true);
    return m_Value;
  }

private:
  bool m_IsValid;
  T m_Value;
};

#endif //Checked_hpp
