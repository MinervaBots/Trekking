#ifndef DISTANCE_TO_CAMERA_H
#define DISTANCE_TO_CAMERA_H

class DistanceToCamera
{
	public:
		DistanceToCamera (float objectRealHeight, float focalLenght, float sensorHeight, int imagePixelHeight);
		float calculate (int objectPixelHeight);

	private:
		float objectRealHeight_;
		float focalLenght_;
		float sensorHeight_;
		int imagePixelHeight_;
};

#endif