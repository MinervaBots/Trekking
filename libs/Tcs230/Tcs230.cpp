#include <Arduino.h>

#include "Tcs230.h"


Tcs230::Tcs230(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3, uint8_t outputEnable, uint8_t frequencyOut)
{
	s0_ = s0;
	s1_ = s1;
	s2_ = s2;
	s3_ = s3;
	outputEnable_ = outputEnable;
	frequencyOut_ = frequencyOut;
}


void Tcs230::setup()
{
	pinMode(s0_, OUTPUT);
	pinMode(s1_, OUTPUT);
	pinMode(s2_, OUTPUT);
	pinMode(s3_, OUTPUT);
	pinMode(outputEnable_, OUTPUT);
	pinMode(frequencyOut_, INPUT);
}


void Tcs230::read()
{
	frequencyReadings_[0] = getFrequency(128);
	setFilter(Tcs230Filter::Red);
	
	frequencyReadings_[1] = getFrequency(128);
	setFilter(Tcs230Filter::Green);
	
	frequencyReadings_[2] = getFrequency(128);
	setFilter(Tcs230Filter::Blue);
	
	frequencyToRgb();
}


void Tcs230::setEnable(bool state)
{
	enabled_ = state;
	digitalWrite(outputEnable_, enabled_ ? LOW : HIGH);
}


void Tcs230::setCalibration(int32_t data[3], bool dark)
{
	if (dark)
	{
		blackCalibration_[0] = data[0];
		blackCalibration_[1] = data[1];
		blackCalibration_[2] = data[2];
	}
	else
	{
		whiteCalibration_[0] = data[0];
		whiteCalibration_[1] = data[1];
		whiteCalibration_[2] = data[2];
	}
}


void Tcs230::setFrequency(Tcs230Frequency frequency)
{
	frequency_ = frequency;
	
	switch (frequency_)
	{
		case Tcs230Frequency::High:
			digitalWrite(s0_, HIGH);
			digitalWrite(s1_, HIGH);
			break;
			
		case Tcs230Frequency::Mid:
			digitalWrite(s0_, HIGH);
			digitalWrite(s1_, LOW);
			break;
			
		case Tcs230Frequency::Low:
			digitalWrite(s0_, LOW);
			digitalWrite(s1_, HIGH);
			break;
			
		case Tcs230Frequency::Off:
			digitalWrite(s0_, LOW);
			digitalWrite(s1_, LOW);
			break;
	}
}


void Tcs230::setFilter(Tcs230Filter filter)
{
	filter_ = filter;
	
	switch (filter_)
	{
		case Tcs230Filter::Red:
			digitalWrite(s2_, LOW);
			digitalWrite(s3_, LOW);
			break;
			
		case Tcs230Filter::Green:
			digitalWrite(s2_, HIGH);
			digitalWrite(s3_, HIGH);
			break;
			
		case Tcs230Filter::Blue:
			digitalWrite(s2_, LOW);
			digitalWrite(s3_, HIGH);
			break;
			
		case Tcs230Filter::Clean:
			digitalWrite(s2_, HIGH);
			digitalWrite(s3_, LOW);
			break;
	}
}


bool Tcs230::isEnabled()
{
	return enabled_;
}

RgbColor Tcs230::getLastReading()
{
	return lastReading_;
}


long Tcs230::getFrequency(uint16_t samples)
{
  long freq = 0;
  for(uint32_t i = 0; i < samples; i++)
	  freq += 500000 / pulseIn(frequencyOut_, HIGH, 250000);
  return freq / samples;
}


long Tcs230::frequencyToRgb()
{
	int32_t	aux;
	
	aux = (frequencyReadings_[0] - blackCalibration_[0]) * 256;
	aux /= (whiteCalibration_[0] - blackCalibration_[0]);
	lastReading_.red = constrain(aux, 0, 255);
	
	aux = (frequencyReadings_[1] - blackCalibration_[1]) * 256;
	aux /= (whiteCalibration_[1] - blackCalibration_[1]);
	lastReading_.blue = constrain(aux, 0, 255);
	
	aux = (frequencyReadings_[2] - blackCalibration_[2]) * 256;
	aux /= (whiteCalibration_[2] - blackCalibration_[2]);
	lastReading_.green = constrain(aux, 0, 255);
}
