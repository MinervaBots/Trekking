#include "DistanceToCamera.h"

DistanceToCamera::DistanceToCamera(float objectRealHeight, float focalLenght, float sensorHeight, int imagePixelHeight)
	: 	objectRealHeight_(objectRealHeight), 
		focalLenght_(focalLenght), 
		sensorHeight_(sensorHeight),
		imagePixelHeight_(imagePixelHeight)
{
}

float DistanceToCamera::calculate (int objectPixelHeight)
{
	return (focalLenght_ * objectRealHeight_ * imagePixelHeight_) / (objectPixelHeight * sensorHeight_);
}
