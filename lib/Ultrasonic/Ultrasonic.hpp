#ifndef Ultrasonic_hpp
#define Ultrasonic_hpp

#include "../InputSource/InputSource.hpp"

class Ultrasonic: public InputSource<unsigned short>
{
public:
	Ultrasonic(int triggerPin, int echoPin);
	Ultrasonic(int triggerPin, int echoPin, unsigned short minRange, unsigned short maxRange);

	unsigned long getTimming();
	unsigned short getInput();

	void setRange(unsigned short minimum, unsigned short maximum);
	double getRawValue();

private:
	unsigned char m_TriggerPin, m_EchoPin;
	unsigned short m_MinRange;
	unsigned short m_MaxRange;
	unsigned long m_Timeout;
};

#endif //Ultrasonic_hpp
