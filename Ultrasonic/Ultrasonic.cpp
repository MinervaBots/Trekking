#include <Arduino.h>

#include "Ultrasonic.h"


Ultrasonic::Ultrasonic(uint8_t triggerPin, uint8_t echoPin) : Ultrasonic(triggerPin, echoPin, 200)
{
}

Ultrasonic::Ultrasonic(uint8_t triggerPin, uint8_t echoPin, uint16_t maxDistance)
{
	triggerPin_ = triggerPin;
	echoPin_ = echoPin;
	timeout_ = CM_TO_MS(maxDistance);
}

bool Ultrasonic::read()
{
	digitalWrite(triggerPin_, LOW);
	delayMicroseconds(2);
	digitalWrite(triggerPin_, HIGH);
	delayMicroseconds(10);
	digitalWrite(triggerPin_, LOW);
	
	long duration = pulseIn(echoPin_, HIGH, timeout_);
	if (duration == 0)
	{
		distance_ = 160000;
		return false;
	}
	
	distance_ =	MS_TO_CM(duration);
	return true;
}

bool Ultrasonic::read(int* distance)
{
	bool returnValue = read();
	*distance = distance_;
	return returnValue;
}

int Ultrasonic::getDistance()
{
	return distance_;
}
