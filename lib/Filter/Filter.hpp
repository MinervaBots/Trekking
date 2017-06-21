#ifndef Filter_hpp
#define Filter_hpp

#include "../InputSource/InputSource.hpp"

template<typename T>
class Filter : public InputSource<T>
{
public:
  virtual T getInput(float rawInput) = 0;
  virtual T getInput() { return getInput(m_pInputSource->getInput()); }
  virtual void setInputSource(InputSource<T>* pInputSource) { m_pInputSource = pInputSource; }

protected:
  InputSource<T> *m_pInputSource;
};

#endif //Filter_hpp
