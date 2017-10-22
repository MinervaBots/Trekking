#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "HaarCascadeDetector.h"


int main()
{
	auto detector = HaarCascadeDetector("C:/Users/paulo/source/repos/OpenCVTry/x64/Debug/haarCascade.xml");
	detector.setScaleFactor(1.1f);
	detector.setFlags(0);
	detector.setMinNeighbors(3);
	detector.enableColorCheck();

	namedWindow("test");
	auto frame = imread("C:/Users/paulo/source/repos/OpenCVTry/x64/Debug/src.jpg");

	auto results = detector.detect(frame);

	for each (IntRect rect in results)
	{
		rectangle(frame, Rect(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight()), Scalar(0, 255, 0, 127), 3);

	}

	printf("Resultados: %zd", results.size());
	imshow("test", frame);
	waitKey(0);
	return 0;
}