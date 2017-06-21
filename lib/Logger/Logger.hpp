#ifndef Logger_hpp
#define Logger_hpp

#include <string.h>
#include <stdarg.h>

enum LogLevel
{
  Nothing,
  Store,
  Errors,
  Assertions,
  Warnings,
  Infos,
  Debugs,
  Verboses
};

class Logger
{
private:
  LogLevel m_LogLevel;

protected:
  virtual void print(const char *format, va_list args = 0) = 0;

public:
  Logger(LogLevel logLevel) : m_LogLevel(logLevel) {}
  void setLogLevel(LogLevel logLevel) { m_LogLevel = logLevel; }

  virtual void store(const char* tag, const char* msg, ...)
  {
    if(LogLevel::Store < m_LogLevel)
    {
      print("[S]<");
      print(tag);
      print(">: ");
      va_list args;
      va_start(args, msg);
      print(msg,args);
    }
  }

  virtual void error(const char* msg, ...)
  {
    if(LogLevel::Errors < m_LogLevel)
    {
      print("[E]: ");
      va_list args;
      va_start(args, msg);
      print(msg,args);
    }
  }

  virtual void assertion(const char* msg, ...)
  {
    if(LogLevel::Assertions < m_LogLevel)
    {
      print("[A]: ");
      va_list args;
      va_start(args, msg);
      print(msg,args);
    }
  }

  virtual void warning(const char* msg, ...)
  {
    if(LogLevel::Warnings < m_LogLevel)
    {
      print("[W]: ");
      va_list args;
      va_start(args, msg);
      print(msg,args);
    }
  }

  virtual void info(const char* msg, ...)
  {
    if(LogLevel::Infos < m_LogLevel)
    {
      print("[I]: ");
      va_list args;
      va_start(args, msg);
      print(msg,args);
    }
  }

  virtual void debug(const char* msg, ...)
  {
    if(LogLevel::Debugs < m_LogLevel)
    {
      print("[D]: ");
      va_list args;
      va_start(args, msg);
      print(msg,args);
    }
  }

  virtual void verbose(const char* msg, ...)
  {
    if(LogLevel::Verboses < m_LogLevel)
    {
      print("[V]: ");
      va_list args;
      va_start(args, msg);
      print(msg,args);
    }
  }
};

extern Logger* Log;

#endif // Logger_hpp
