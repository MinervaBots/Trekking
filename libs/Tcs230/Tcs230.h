#ifndef TCS230_H
#define TCS230_H

enum Tcs230Frequency
{
	High, // 100%
	Mid,  // 20%
	Low,  // 2%
	Off   // 0%
};

enum Tcs230Filter
{
	Red,
	Green,
	Blue,
	Clean
};

struct RgbColor
{
	uint8_t red, green, blue;
	
	uint8_t greyScale()
	{
		return (red + green + blue) / 3;
	}
};

class Tcs230 
{
public:
	Tcs230(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3, uint8_t outputEnable, uint8_t frequencyOut);
	
	void setup();
	void read();
	
	void setEnable(bool state);
	void setCalibration(int32_t data[3], bool dark);
	void setFrequency(Tcs230Frequency frequency);
	void setFilter(Tcs230Filter filter);
	
	bool isEnabled();
	RgbColor getLastReading();
	
private:
	uint8_t s0_, s1_, s2_, s3_, outputEnable_, frequencyOut_;
	
	bool enabled_;
	
	int32_t blackCalibration_[3], whiteCalibration_[3];
	
	Tcs230Filter filter_;
	Tcs230Frequency frequency_;
	
	int32_t frequencyReadings_[3];
	RgbColor lastReading_;
	
	
	long getFrequency(uint16_t samples);
	long frequencyToRgb();
};


#endif // TCS230_H