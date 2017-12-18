#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <ctime>

#include "HaarCascadeDetector.h"
 
using namespace cv;
using namespace std;

int main()
{

	// Configura o detector
	auto detector = HaarCascadeDetector("haarCascade.xml");
	detector.setScaleFactor(1.08f);
	detector.setFlags(0);
	detector.setMinNeighbors(1);
	detector.setColorThresHold(50, Scalar(0, 65, 45), Scalar(30, 255, 255));
	detector.enableColorCheck();

	// Abre a câmera padrão
	VideoCapture cap(0);
	// Checa se ela está conetada
	if(!cap.isOpened())
	{
		cerr << "ERROR: Unable to open the camera" << endl;
		// Serial.Error("Não foi possivel abrir a câmera");
		return -1;
	}

	cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,360);


	bool running = true;
	Mat frame;
	while(running)
	{
		clock_t begin = clock();
		// Pega um frame da câmera e decodifica
		//cap.read(&frame);
		cap >> frame;
		
		if (frame.empty()) {
			cerr << "ERROR: Unable to grab from the camera" << endl;
			running = false;
		}
		int key = cv::waitKey(5);
		key = (key==255) ? -1 : key; //#Solve bug in 3.2.0
		if (key>=0)
	  		running = false;
   	
		auto results = detector.detect(frame);

		for (IntRect rect : results)
		{
			rectangle(frame, Rect(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight()), Scalar(0, 255, 0, 127), 3);
		}
/*
		// Mostra os resultados
		printf("\rResultados: %zd", results.size());
*/

		imshow("Live",frame);
		clock_t end = clock();
		double elapsed_secs = (double(end - begin) / CLOCKS_PER_SEC) * 1000;
		printf("Elapsed %f\n", elapsed_secs);
	}
	cap.release();
	destroyAllWindows();
	return 0;
}