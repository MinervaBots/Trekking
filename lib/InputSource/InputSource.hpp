#ifndef InputSource_hpp
#define InputSource_hpp

template<typename T>
class InputSource
{
public:
  virtual T getInput() = 0;
};

#endif // InputSource_hpp
