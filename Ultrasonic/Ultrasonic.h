#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#define CM_TO_MS(cm) cm * 58
#define MS_TO_CM(ms) ms / 58

class Ultrasonic
{
public:
    Ultrasonic(uint8_t triggerPin, uint8_t echoPin);
	Ultrasonic(uint8_t triggerPin, uint8_t echoPin, uint16_t maxDistance);
	
	bool read();
	bool read(int* distance);
	
	int getDistance();

private:
    uint8_t triggerPin_, echoPin_;
	long timeout_;
	
	int distance_;
};


#endif // ULTRASONIC_H
