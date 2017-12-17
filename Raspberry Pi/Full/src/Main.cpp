#include "HaarCascadeDetector.h"

int main()
{
	// Configura o detector
	auto detector = HaarCascadeDetector("C:/Users/paulo/source/repos/OpenCVTry/x64/Debug/haarCascade.xml");
	detector.setScaleFactor(1.08f);
	detector.setFlags(0);
	detector.setMinNeighbors(1);
	detector.setColorThresHold(50, Scalar(0, 65, 45), Scalar(30, 255, 255));
	detector.enableColorCheck();

	namedWindow("coloredView");

	// Abre a câmera padrão
	VideoCapture cap(0);
	// Checa se ela está conetada
    if(!cap.isOpened())
    {
    	// Serial.Error("Não foi possivel abrir a câmera");
        return -1;
    }

    while(running)
    {
    	Mat frame;
    	// Pega um frame da câmera e decodifica
    	cap.read(&frame);

    	auto results = detector.detect(frame);

    	for each (IntRect rect in results)
		{
			rectangle(frame, Rect(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight()), Scalar(0, 255, 0, 127), 3);
		}

		// Mostra os resultados
		printf("\rResultados: %zd", results.size());
		imshow("coloredView", frame);
    }
}