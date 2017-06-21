#ifndef PrintLogger_hpp
#define PrintLogger_hpp

#include "Logger.hpp"
#include <stdio.h>
#include <stdarg.h>
#include <Arduino.h>

class PrintLogger : public Logger
{
private:
  Stream *m_pStream;

public:
  PrintLogger(Stream &pStream, LogLevel logLevel) : Logger(logLevel), m_pStream(&pStream) {}
  void setStream(Stream &pStream) { m_pStream = &pStream; }

protected:
  void print(const char *format, va_list args = 0)
  {
    for (; *format != 0; ++format)
    {
      if (*format == '%')
      {
        ++format;
        if (*format == '\0') break;
        if (*format == '%')
        {
          m_pStream->print(*format);
          continue;
        }
        if(*format == 's')
        {
  				register char *s = (char *)va_arg(args, int);
  				m_pStream->print(s);
  				continue;
  			}
        if(*format == 'd' || *format == 'i')
        {
  				m_pStream->print(va_arg(args, int), DEC);
  				continue;
  			}
        if( *format == 'x' )
        {
  				m_pStream->print(va_arg(args, int), HEX);
  				continue;
			  }
        if(*format == 'X')
        {
    			m_pStream->print("0x");
    			m_pStream->print(va_arg(args, int),HEX);
    			continue;
			  }
        if(*format == 'b')
        {
  				m_pStream->print(va_arg(args, int),BIN);
  				continue;
        }
        if(*format == 'B')
        {
        	m_pStream->print("0b");
        	m_pStream->print(va_arg(args, int), BIN);
        	continue;
        }
        if(*format == 'l')
        {
  				m_pStream->print(va_arg(args, long), DEC);
  				continue;
  			}

        if(*format == 'c')
        {
  				m_pStream->print(va_arg(args, int));
  				continue;
  			}
        if(*format == 't')
        {
  				if (va_arg(args, int) == 1)
          {
  					m_pStream->print("T");
  				}
  				else
          {
  					m_pStream->print("F");
  				}
  				continue;
  			}
        if(*format == 'T')
        {
  				if (va_arg(args, int) == 1)
          {
  					m_pStream->print("true");
  				}
  				else
          {
  					m_pStream->print("false");
  				}
          continue;
        }
      }
      m_pStream->print(*format);
    }
  }
};

#endif //PrintLogger_hpp
