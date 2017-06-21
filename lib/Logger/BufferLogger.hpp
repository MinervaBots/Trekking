#ifndef BufferLogger_hpp
#define BufferLogger_hpp

#include "Logger.hpp"

template<unsigned short BufferSize>
class BufferLogger : public Logger
{
public:
  BufferLogger();
  ~BufferLogger();
  void write(const char* format, ...);
  void writeLine(const char* format, ...);
  virtual void flush();
  const char *getBuffer() { return m_LogsBuffer; }
private:
  char m_LogsBuffer[BufferSize];
  unsigned int m_BufferPosition;
};

#endif // BufferLogger_hpp
