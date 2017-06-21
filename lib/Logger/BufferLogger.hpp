#ifndef BufferLogger_hpp
#define BufferLogger_hpp

#include "Logger.hpp"

template<unsigned short BufferSize>
class BufferLogger : public Logger
{
private:
  char m_LogsBuffer[BufferSize];
  unsigned int m_BufferPosition;

protected:
  void print(const char *format, va_list args = 0)
  {
    /*
    O tamanho disponível é o total do buffer menos o que já tá ocupado.
    Subtrai 1 aqui pra garantir que vai ter espaço pro marcador de fim de string.
    */
    unsigned int availabeSize = BufferSize - m_BufferPosition - 1;
    vsnprintf (&m_LogsBuffer[m_BufferPosition], availabeSize, format, args);
    va_end (args);

    unsigned int lenght;
    for (lenght = 0; lenght < availabeSize; lenght++)
    {
      if(m_LogsBuffer[m_BufferPosition + lenght] == '\0')
        break;
    }
    // Incrementa o tamanho da string que foi adicionado
    m_BufferPosition += lenght;

    /*
    Posiciona o marcador de fim de string logo no final do que foi adicionado agora
    Se algo for escrito depois vai sobrescrever esse marcador.

    Por causa de flush(), teoricamente tudo depois disso é '\0' então não precisa
    se preocupar com marcador de fim de string.
    */
    //m_LogsBuffer[m_BufferPosition] = '\0';
  }

public:
  BufferLogger(LogLevel logLevel) : Logger(logLevel) {}
  ~BufferLogger();
  virtual void flush();
  const char *getBuffer() { return m_LogsBuffer; }
};


#endif // BufferLogger_hpp
