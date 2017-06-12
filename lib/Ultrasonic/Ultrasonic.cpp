#include "Ultrasonic.hpp"
#include <Arduino.h>

Ultrasonic::Ultrasonic(int triggerPin, int echoPin) :
	m_TriggerPin(triggerPin),
	m_EchoPin(echoPin)
{
	pinMode(m_TriggerPin, OUTPUT);
	pinMode(m_EchoPin, INPUT);
	setRange(30, 200); // 30cm a 2m (200cm)
}

Ultrasonic::Ultrasonic(int triggerPin, int echoPin, unsigned short minRange, unsigned short maxRange) :
	Ultrasonic(triggerPin, echoPin)
{
		setRange(minRange, maxRange);
}

unsigned long Ultrasonic::getTimming()
{
	digitalWrite(m_TriggerPin, LOW);
	delayMicroseconds(2);

	digitalWrite(m_TriggerPin, HIGH);
	delayMicroseconds(10);

	digitalWrite(m_TriggerPin, LOW);

	return pulseIn(m_EchoPin, HIGH, m_Timeout);
}

unsigned short Ultrasonic::getInput()
{
	unsigned short distance = getTimming() / 58.2;

	if(distance <= m_MinRange || distance >= m_MaxRange)
	{
		return -1; //OutOfRange
	}
	return distance;
}

void Ultrasonic::setRange(unsigned short minimum, unsigned short maximum)
{
	m_MinRange = minimum;
	m_MaxRange = maximum;
	m_Timeout = maximum * 58.2;
}
