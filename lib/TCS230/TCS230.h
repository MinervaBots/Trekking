/*
	  TCS230.h - TCS230 implements trigger and digital read methods for RGB
 *  color detection, implements a particular method that says if an object
 *  is white or not.
	  Created by Edilson Fernandes, July, 2016.
	  Read LICENSE for more information.
*/


#ifndef TCS230_H
#define TCS230_H

#include "../CustomTypes/Color.hpp"
#include "../InputSource/InputSource.hpp"

class TCS230 : public InputSource<Color>
{
public:
  TCS230(int s0_pin, int s1_pin, int s2_pin, int s3_pin, int out_pin, int WHITE_VALUE);
  Color getInput();

private:
  Color m_ColorCache;

  //These methods are getters for each primary color;
  float getRed();
  float getGreen();
  float getBlue();
  float getWhite();

  //This method returns if an object is white or not;
  bool isWhite();
  void setWhiteValue(int value);

  //*****************************************************************************************
  void printRGBW(float r, float g, float b, float w);


  //Variables used to store the minimum and the maximum values, between 20 readings, for each color;
  float max_red;
  float max_green;
  float max_blue;
  float min_red;
  float min_green;
  float min_blue;
  //Variables that keep the pins values you've choose;
  int s0_pin;
  int s1_pin;
  int s2_pin;
  int s3_pin;
  int out_pin;
  int white_value;
 };

#endif /* TCS230_H */
