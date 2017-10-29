#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "HaarCascadeDetector.h"
#include <ctime>

int main()
{
	auto detector = HaarCascadeDetector("C:/Users/paulo/source/repos/OpenCVTry/x64/Debug/haarCascade.xml");
	detector.setScaleFactor(1.08f);
	detector.setFlags(0);
	detector.setMinNeighbors(1);
	detector.setColorThresHold(50, Scalar(0, 65, 45), Scalar(30, 255, 255));
	detector.enableColorCheck();
	//detector.setMaxSize(300, 300);

	namedWindow("test");

	clock_t begin = clock();
	auto frame = imread("C:/Users/paulo/source/repos/OpenCVTry/x64/Debug/src4.jpg");
	auto results = detector.detect(frame);
	clock_t end = clock();
	double elapsed_secs = (double(end - begin) / CLOCKS_PER_SEC) * 1000;
	printf("Elapsed %f\n", elapsed_secs);

	for each (IntRect rect in results)
	{
		rectangle(frame, Rect(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight()), Scalar(0, 255, 0, 127), 3);
	}

	printf("Resultados: %zd", results.size());
	imshow("test", frame);
	waitKey(0);
	return 0;
}