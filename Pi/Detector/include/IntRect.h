#pragma once

#include "IntVector2.h"

struct IntRect : public IntVector2
{
	IntRect(int x, int y, int width, int height) : IntVector2(x, y), extension_(width, height) {  }

	int getWidth() { return extension_.getX(); }
	int getHeight() { return extension_.getY(); }

	void setWidth(int width) { extension_.setX(width); }
	void setHeight(int height) { extension_.setY(height); }

private:
	IntVector2 extension_;
};