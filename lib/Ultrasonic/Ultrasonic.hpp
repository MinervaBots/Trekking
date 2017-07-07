#ifndef Ultrasonic_hpp
#define Ultrasonic_hpp

#include "../InputSource/InputSource.hpp"

class Ultrasonic: public InputSource<float>
{
public:
	Ultrasonic(int triggerPin, int echoPin);
	Ultrasonic(int triggerPin, int echoPin, unsigned short minRange, unsigned short maxRange);

	void trigger();
	float getInput();
	void setRange(unsigned short minimum, unsigned short maximum);

private:
	unsigned char m_TriggerPin, m_EchoPin;
	unsigned short m_MinRange;
	unsigned short m_MaxRange;
	unsigned long m_Timeout;
	float m_Distance;
};

#endif //Ultrasonic_hpp
