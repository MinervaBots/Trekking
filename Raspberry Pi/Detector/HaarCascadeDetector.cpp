#include "stdafx.h"
#include "HaarCascadeDetector.h"

HaarCascadeDetector::HaarCascadeDetector(String pathToCascadeFile)
{
	if (!cascadeClassifier_.load(pathToCascadeFile))
	{
		CV_Error(1, "Cascade file not found");
	}
}

void HaarCascadeDetector::setColorThresHold(double colorPresenceMin, Scalar thresholdLow, Scalar thresholdHigh)
{
	colorPresenceMin_ = colorPresenceMin;
	colorThresholdLow_ = thresholdLow;
	colorThresholdHigh_ = thresholdHigh;
}

void HaarCascadeDetector::setScaleFactor(double value)
{
	scaleFactor_ = value;
}

void HaarCascadeDetector::setMinNeighbors(int value)
{
	minNeighbors_ = value;
}

void HaarCascadeDetector::setFlags(int value)
{
	flags_ = value;
}

void HaarCascadeDetector::setMinSize(int width, int height)
{
	minObjectSize_ = Size(width, height);
}

void HaarCascadeDetector::setMaxSize(int width, int height)
{
	maxObjectSize_ = Size(width, height);
}

void HaarCascadeDetector::enableColorCheck()
{
	//useColorInformation_ = true;
}

void HaarCascadeDetector::disableColorCheck()
{
	useColorInformation_ = false;
}

std::vector<IntRect> HaarCascadeDetector::detect(Mat frame)
{
	std::vector<IntRect> result;
	std::vector<Rect> objects;

	// Detec��o por haar cascade n�o utiliza informa��o de cor,
	// ent�o precisamos recriar o frame em escala de cinza.
	Mat frameInGreyScale;
	cvtColor(frame, frameInGreyScale, COLOR_BGR2GRAY);
	equalizeHist(frameInGreyScale, frameInGreyScale);

	cascadeClassifier_.detectMultiScale(frameInGreyScale, objects, scaleFactor_, minNeighbors_, flags_, minObjectSize_, maxObjectSize_);
	
	for (size_t i = 0; i < objects.size(); i++)
	{
		Rect rect = objects[i];

		if (useColorInformation_)
		{
			// Extrai o peda�o do frame em que o objeto foi detectado por haar cascade.
			// A extra��o � feita do frame original, pois aqui queremos usar as informa��es de cor.
			Mat objectInframe = frame(rect);

			// Calcula uma mask pro intervalo de cor definido.
			// Onde na imagem a cor estiver dentro dos limites fica em 1 (branco) na matriz 
			// e onde n�o estiver fica 0 (preto).
			Scalar colorMask;
			inRange(objectInframe, colorThresholdLow_, colorThresholdHigh_, colorMask);

			// Verifica se a m�dia da mask um valor configurado.
			// Se for indica que a cor est� bem presente na imagem.
			// Aumentando muito as chances de ser o objeto que procuramos j� que o haar cascade
			// n�o leva em considera��o as informa��es de cor
			if (mean(colorMask)[0] < colorPresenceMin_)
			{
				// Se a m�dia for menor que o m�nimo desconsidera essa objeto e pula para o proximo.
				continue;
			}
		}

		result.push_back(IntRect(rect.x, rect.y, rect.width, rect.height));
	}
	return result;
}
