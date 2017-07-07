#include "Ultrasonic.hpp"
#include <Arduino.h>

const float SONAR_TO_CM = 100.0 / 95.0;

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

void Ultrasonic::trigger()
{
	digitalWrite(m_TriggerPin, HIGH);
	delayMicroseconds(20);
	digitalWrite(m_TriggerPin, LOW);
}

float Ultrasonic::getInput()
{
	//m_Distance = analogRead(m_EchoPin) * SONAR_TO_CM; //;* 5 / 1024;// / 58.2;
	m_Distance = (analogRead(m_EchoPin) / 2) * 2.54;// / 58.2;
	//Serial.println(m_Distance);
	/*
	if(m_Distance <= m_MinRange || m_Distance >= m_MaxRange)
	{
		m_Distance = -1;
	}
	*/
	/*
	Serial.println(m_MinRange);
	Serial.println(m_MaxRange);
	Serial.println(m_Timeout);
	*/
	return m_Distance;
}

void Ultrasonic::setRange(unsigned short minimum, unsigned short maximum)
{
	m_MinRange = minimum;
	m_MaxRange = maximum;
	m_Timeout = maximum * 58.2;
}
