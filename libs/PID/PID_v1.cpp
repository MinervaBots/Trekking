/**********************************************************************************************
   Arduino PID Library - Version 1.1.1
   by Brett Beauregard <br3ttb@gmail.com> brettbeauregard.com

   This Library is licensed under a GPLv3 License
 **********************************************************************************************/

#include "Arduino.h"

#include "PID_v1.h"

/*Constructor (...)*********************************************************
      The parameters specified here are those for for which we can't set up
      reliable defaults, so we need to have the user set them.
 ***************************************************************************/
PID::PID(float* input, float* output, float* setpoint,
         float Kp, float Ki, float Kd, int POn, int controllerDirection)
{
  output_ = output;
  input_ = input;
  setpoint_ = setpoint;
  inAuto = false;

  PID::setOutputLimits(0, 255);				//default output limit corresponds to
  //the arduino pwm limits

  sampleTime = 100;							//default Controller Sample Time is 0.1 seconds

  PID::setControllerDirection(controllerDirection);
  PID::setTunings(Kp, Ki, Kd, POn);

  lastTime = millis() - sampleTime;
}

/*Constructor (...)*********************************************************
      To allow backwards compatability for v1.1, or for people that just want
      to use Proportional on Error without explicitly saying so
 ***************************************************************************/

PID::PID(float* input, float* output, float* setpoint,
         float Kp, float Ki, float Kd, int controllerDirection)
  : PID::PID(input, output, setpoint, Kp, Ki, Kd, P_ON_E, controllerDirection)
{

}


/* Compute() **********************************************************************
       This, as they say, is where the magic happens.  this function should be called
     every time "void loop()" executes.  the function will decide for itself whether a new
     pid Output needs to be computed.  returns true when the output is computed,
     false when nothing has been done.
 **********************************************************************************/
bool PID::compute()
{
  if (!inAuto)
    return false;

  unsigned long now = millis();
  unsigned long timeChange = (now - lastTime);

  if (timeChange >= sampleTime)
  {
    /*Compute all the working error variables*/
    float input = *input_;
    float error = *setpoint_ - input;
    float dInput = (input - lastInput);
    outputSum += (ki * error);

    /*Add Proportional on Measurement, if P_ON_M is specified*/
    if (!pOnE)
      outputSum -= kp * dInput;

    if (outputSum > outMax)
      outputSum = outMax;

    else if (outputSum < outMin)
      outputSum = outMin;

    /*Add Proportional on Error, if P_ON_E is specified*/
    float output;

    if (pOnE)
      output = kp * error;

    else
      output = 0;

    /*Compute Rest of PID Output*/
    output += outputSum - kd * dInput;

    if (output > outMax)
      output = outMax;

    else if (output < outMin)
      output = outMin;
      
    *output_ = output;

    /*Remember some variables for next time*/
    lastInput = input;
    lastTime = now;
    return true;
  }
  else return false;
}

/* SetTunings(...)*************************************************************
   This function allows the controller's dynamic performance to be adjusted.
   it's called automatically from the constructor, but tunings can also
   be adjusted on the fly during normal operation
 ******************************************************************************/
void PID::setTunings(float Kp, float Ki, float Kd, int POn)
{
  if (Kp < 0 || Ki < 0 || Kd < 0)
    return;

  pOn = POn;
  pOnE = POn == P_ON_E;

  dispKp = Kp;
  dispKi = Ki;
  dispKd = Kd;

  float SampleTimeInSec = ((float)sampleTime) / 1000;
  kp = Kp;
  ki = Ki * SampleTimeInSec;
  kd = Kd / SampleTimeInSec;

  if (controllerDirection == REVERSE)
  {
    kp = (0 - kp);
    ki = (0 - ki);
    kd = (0 - kd);
  }
}

/* SetTunings(...)*************************************************************
   Set Tunings using the last-rembered POn setting
 ******************************************************************************/
void PID::setTunings(float Kp, float Ki, float Kd)
{
  setTunings(Kp, Ki, Kd, pOn);
}

/* setSampleTime(...) *********************************************************
   sets the period, in Milliseconds, at which the calculation is performed
 ******************************************************************************/
void PID::setSampleTime(int newSampleTime)
{
  if (newSampleTime > 0)
  {
    float ratio  = (float) newSampleTime
                   / (float) sampleTime;
    ki *= ratio;
    kd /= ratio;
    sampleTime = (unsigned long) newSampleTime;
  }
}

/* SetOutputLimits(...)****************************************************
       This function will be used far more often than SetInputLimits.  while
    the input to the controller will generally be in the 0-1023 range (which is
    the default already,)  the output will be a little different.  maybe they'll
    be doing a time window and will need 0-8000 or something.  or maybe they'll
    want to clamp it from 0-125.  who knows.  at any rate, that can all be done
    here.
 **************************************************************************/
void PID::setOutputLimits(float Min, float Max)
{
  if (Min >= Max)
    return;

  outMin = Min;
  outMax = Max;

  if (inAuto)
  {
    if (*output_ > outMax)
      *output_ = outMax;

    else if (*output_ < outMin)
      *output_ = outMin;

    if (outputSum > outMax)
      outputSum = outMax;

    else if (outputSum < outMin)
      outputSum = outMin;
  }
}

/* SetMode(...)****************************************************************
   Allows the controller Mode to be set to manual (0) or Automatic (non-zero)
   when the transition from manual to auto occurs, the controller is
   automatically initialized
 ******************************************************************************/
void PID::setMode(int Mode)
{
  bool newAuto = (Mode == AUTOMATIC);
  if (newAuto && !inAuto)
  { /*we just went from manual to auto*/
    PID::initialize();
  }
  inAuto = newAuto;
}

/* Initialize()****************************************************************
 	does all the things that need to happen to ensure a bumpless transfer
    from manual to automatic mode.
 ******************************************************************************/
void PID::initialize()
{
  outputSum = *output_;
  lastInput = *input_;

  if (outputSum > outMax)
    outputSum = outMax;

  else if (outputSum < outMin)
    outputSum = outMin;
}

/* SetControllerDirection(...)*************************************************
   The PID will either be connected to a DIRECT acting process (+Output leads
   to +Input) or a REVERSE acting process(+Output leads to -Input.)  we need to
   know which one, because otherwise we may increase the output when we should
   be decreasing.  This is called from the constructor.
 ******************************************************************************/
void PID::setControllerDirection(int direction)
{
  if (inAuto && direction != controllerDirection)
  {
    kp = (0 - kp);
    ki = (0 - ki);
    kd = (0 - kd);
  }
  
  controllerDirection = direction;
}

/* Status Funcions*************************************************************
   Just because you set the Kp=-1 doesn't mean it actually happened.  these
   functions query the internal state of the PID.  they're here for display
   purposes.  this are the functions the PID Front-end uses for example
 ******************************************************************************/
float PID::getKp()
{
  return dispKp;
}

float PID::getKi()
{
  return dispKi;
}

float PID::getKd()
{
  return dispKd;
}

int PID::getMode()
{
  return inAuto ? AUTOMATIC : MANUAL;
}

int PID::getDirection()
{
  return controllerDirection;
}
