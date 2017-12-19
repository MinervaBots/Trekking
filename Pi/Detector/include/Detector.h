#pragma once

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "IntVector2.h"
#include "IntRect.h"


using namespace cv;


class IDectector
{
public:
	virtual std::vector<IntRect> detect(Mat frame) = 0;
};