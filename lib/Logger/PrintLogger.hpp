#ifndef PrintLogger_hpp
#define PrintLogger_hpp

#include <stdio.h>
#include <stdarg.h>
#include <Arduino.h>


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

class PrintLogger : public Print
{
	using Print::print;
	using Print::println;

private:
  Stream *m_pStream;
  LogLevel m_LogLevel;

public:
  PrintLogger(Stream &pStream, LogLevel logLevel) : m_pStream(&pStream), m_LogLevel(logLevel) {}
	void setLogLevel(LogLevel logLevel) { m_LogLevel = logLevel; }
  void setStream(Stream &pStream) { m_pStream = &pStream; }

  template<typename T>
  void debug(const char* tag, T* msg)
  {
    println(LogLevel::Debugs, tag, msg);

  }

protected:
  /*
  		Calls the write method from the current stream
  	*/
  	size_t write(uint8_t b) {
  		size_t bytes_written = 0;
  		bytes_written = m_pStream->write(b);
  		return bytes_written;
  	}

  	/*
  		Calls the write method from the current stream
  	*/
  	size_t write(const uint8_t *buffer, size_t size) {
  		size_t bytes_written = 0;
  		bytes_written = m_pStream->write(buffer, size);
  		return bytes_written;
  	}

  	/*
  		Prints the priority, the tag and the message in the following pattern:

  		priority/tag: message

  		and returns the number of bytes written. The message will be printed if:
  		- The log object is enabled and
  		- The message's priority is the same as the current priority or
  		- The current priority is setted to verbose
  	*/
  	template <typename T>

  	size_t print(LogLevel priority, char const * tag, T msg, bool print_header=true) {

  		size_t bytes_written = 0;

  		if(priority < m_LogLevel) {
  			if(print_header) {
  				String full_message = "";
  				full_message += (char)priority;
  				full_message += "/";
  				full_message += tag;
  				full_message += ": ";
  				bytes_written += print(full_message);
  			}
  			bytes_written += print(msg);
  		}

  		return bytes_written;
  	}

  	/*
  		Calls the print method, prints the new line character "\n"
  		and returns the number of bytes written
  	*/
  	template <typename T>

  	size_t println(LogLevel priority, char const * tag, T msg) {

  		size_t bytes_written = print(priority, tag, msg);

  		if(bytes_written) {
  			bytes_written += print("\n");
  		}

  		return bytes_written;
  	}
};

extern PrintLogger Log;

#endif //PrintLogger_hpp
