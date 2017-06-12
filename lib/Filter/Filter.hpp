#ifndef Filter_hpp
#define Filter_hpp

#include "../InputSource/InputSource.hpp"

template<typename T>
class Filter : public InputSource<T>
{
public:
  virtual T getInput() = 0;
  virtual void setInputSource(InputSource<T>* pInputSource) { m_pInputSource = pInputSource; }

protected:
  InputSource<T> *m_pInputSource;
};

#endif //Filter_hpp
