#ifndef RC_RECEIVER_H
#define RC_RECEIVER_H

void attachRCInterrupts();


void gearISR();
void ruddISR();
void elevISR();

extern volatile unsigned long lastSignalTime;
extern volatile unsigned long gearPulseWidth;
extern volatile unsigned long ruddPulseWidth;
extern volatile unsigned long elevPulseWidth;


#endif // RC_RECEIVER_H