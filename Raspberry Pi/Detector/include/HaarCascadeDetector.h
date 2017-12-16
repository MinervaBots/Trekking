#pragma once

#include "Detector.h"

#include <opencv2/objdetect.hpp>


using namespace std;

class HaarCascadeDetector : public IDectector
{
public:
	HaarCascadeDetector(String pathToCascadeFile);

	void setColorThresHold(double colorPresenceMin, Scalar thresholdLow, Scalar thresholdHigh);
	void setScaleFactor(double value);
	void setMinNeighbors(int value);
	void setFlags(int value);
	void setMinSize(int width, int height);
	void setMaxSize(int width, int height);

	void enableColorCheck();
	void disableColorCheck();

	std::vector<IntRect> detect(Mat frame) override;

private:
	CascadeClassifier cascadeClassifier_;

	double scaleFactor_ = 1.1;
	int minNeighbors_ = 3;
	int flags_ = 0;
	Size minObjectSize_ = Size();
	Size maxObjectSize_ = Size();

	bool useColorInformation_ = false;
	Scalar colorThresholdLow_;
	Scalar colorThresholdHigh_;
	double colorPresenceMin_;
};