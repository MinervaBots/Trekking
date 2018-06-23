#include <Arduino.h>
#include "Pins.h"
#include "RCReceiver.h"
#include "Utils.h"

volatile unsigned long lastSignalTime = 0;
volatile unsigned long gearRiseTime = 0, gearPulseWidth = 0;
volatile unsigned long ruddRiseTime = 0, ruddPulseWidth = 0;
volatile unsigned long elevRiseTime = 0, elevPulseWidth = 0;

void gearRising();
void gearFalling();

void ruddRising();
void ruddFalling();

void elevRising();
void elevFalling();

void attachRCInterrupts()
{
    pinMode(RUDD_PIN, INPUT_PULLUP);
    pinMode(ELEV_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(RUDD_PIN), ruddISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ELEV_PIN), elevISR, CHANGE);
    setupChangeInterrupt(GEAR_PIN);
}

void gearISR()
{
    if (gearRiseTime == 0)
    {
        gearRising();
    }
    else
    {
        gearFalling();
    }
    lastSignalTime = millis();
}

void ruddISR()
{
    if (ruddRiseTime == 0)
    {
        ruddRising();
    }
    else
    {
        ruddFalling();
    }
    lastSignalTime = millis();
}

void elevISR()
{
    if (elevRiseTime == 0)
    {
        elevRising();
    }
    else
    {
        elevFalling();
    }
    lastSignalTime = millis();
}

void gearRising()
{
    noInterrupts();
    gearRiseTime = micros();
    interrupts();
}

void gearFalling()
{
    noInterrupts();
    gearPulseWidth = micros() - gearRiseTime;
    gearRiseTime = 0;
    interrupts();
}

void ruddRising()
{
    noInterrupts();
    ruddRiseTime = micros();
    interrupts();
}

void ruddFalling()
{
    noInterrupts();
    ruddPulseWidth = micros() - ruddRiseTime;
    ruddRiseTime = 0;
    interrupts();
}

void elevRising()
{
    noInterrupts();
    elevRiseTime = micros();
    interrupts();
}

void elevFalling()
{
    noInterrupts();
    elevPulseWidth = micros() - elevRiseTime;
    elevRiseTime = 0;
    interrupts();
}