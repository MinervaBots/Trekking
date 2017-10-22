#pragma once

#include <opencv2\opencv.hpp>
#include "IntVector2.h"
#include "IntRect.h"


using namespace cv;


class IDectector
{
public:
	virtual std::vector<IntRect> detect(Mat frame) = 0;
};