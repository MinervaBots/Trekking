#ifndef PrintLogger_hpp
#define PrintLogger_hpp

#include "Logger.hpp"
#include <stdio.h>
#include <stdarg.h>
#include <Arduino.h>

class PrintLogger : public Logger
{
public:
  void write(const char* format, ...)
  {
    char formatedMessage[50];
    va_list args;
    va_start (args, format);
    vsnprintf (&formatedMessage[0], sizeof(formatedMessage), format, args);
    va_end (args);

    m_pPrinter->print(formatedMessage);
  }
  void writeLine(const char* format, ...)
  {
    write(format);
    m_pPrinter->print("\n");
  }

  void setPrinter(Print &pPrinter) { m_pPrinter = &pPrinter; }

private:
  Print *m_pPrinter;
};

#endif //PrintLogger_hpp
