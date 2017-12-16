#pragma once

struct IntVector2
{
public:
	IntVector2(int x, int y) : x_(x), y_(y) { }

	int getX() { return x_; }
	int getY() { return y_; }

	void setX(int x) { x_ = x; }
	void setY(int y) { y_ = y; }


private:
	int x_, y_;
};