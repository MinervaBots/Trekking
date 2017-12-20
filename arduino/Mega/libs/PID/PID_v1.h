#ifndef PID_v1_h
#define PID_v1_h

#define LIBRARY_VERSION	1.1.1

class PID
{
  public:

  //Constants used in some of the functions below
  #define AUTOMATIC	1
  #define MANUAL	0
  #define DIRECT  0
  #define REVERSE  1
  #define P_ON_M 0
  #define P_ON_E 1

  //commonly used functions **************************************************************************
    PID(float*, float*, float*,        // * constructor.  links the PID to the Input, Output, and 
        float, float, float, int, int);//   Setpoint.  Initial tuning parameters are also set here.
                                          //   (overload for specifying proportional mode)

    PID(float*, float*, float*,        // * constructor.  links the PID to the Input, Output, and 
        float, float, float, int);     //   Setpoint.  Initial tuning parameters are also set here
	
    void setMode(int Mode);               // * sets PID to either Manual (0) or Auto (non-0)

    bool compute();                       // * performs the PID calculation.  It should be
                                          //   called every time loop() cycles. ON/OFF and
                                          //   calculation frequency can be set using SetMode
                                          //   SetSampleTime respectively

    void setOutputLimits(float, float); // * clamps the output to a specific range. 0-255 by default, but
										                      //   it's likely the user will want to change this depending on
										                      //   the application
	


  //available but not commonly used functions ********************************************************
    void setTunings(float, float,       // * While most users will set the tunings once in the 
                    float);         	    //   constructor, this function gives the user the option
                                          //   of changing tunings during runtime for Adaptive control
    void setTunings(float, float,       // * overload for specifying proportional mode
                    float, int);         	  

	  void setControllerDirection(int);	  // * Sets the Direction, or "Action" of the controller. DIRECT
										  //   means the output will increase when error is positive. REVERSE
										  //   means the opposite.  it's very unlikely that this will be needed
										  //   once it is set in the constructor.
    void setSampleTime(int);              // * sets the frequency, in Milliseconds, with which 
                                          //   the PID calculation is performed.  default is 100
										  
										  
										  
    //Display functions ****************************************************************
	  float getKp();						  // These functions query the pid for interal values.
	  float getKi();						  //  they were created mainly for the pid front-end,
	  float getKd();						  // where it's important to know what is actually 
	  int getMode();						  //  inside the PID.
	  int getDirection();					  //

  private:
	  void initialize();
	
	  float dispKp;				// * we'll hold on to the tuning parameters in user-entered 
	  float dispKi;				//   format for display purposes
	  float dispKd;				//
    
	  float kp;                  // * (P)roportional Tuning Parameter
    float ki;                  // * (I)ntegral Tuning Parameter
    float kd;                  // * (D)erivative Tuning Parameter

	  int controllerDirection;
	  int pOn;

    float *input_;              // * Pointers to the Input, Output, and Setpoint variables
    float *output_;             //   This creates a hard link between the variables and the 
    float *setpoint_;           //   PID, freeing the user from having to constantly tell us
                                //   what these values are.  with pointers we'll just know.
			  
	  unsigned long lastTime;
	  float outputSum, lastInput;

	  unsigned long sampleTime;
	  float outMin, outMax;
	  bool inAuto, pOnE;
};

#endif // PID_v1_h
