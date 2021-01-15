/*
  Meassure.h - Meassure-Library for Airduino.
*/

#ifndef Meassure_h
#define Meassure_h

#include "Arduino.h"
#include "../util/Util.h"


class Meassure {

  public:
    //setup
    static void setup();
    static void loop();
    static State getState();
    static int getAirCondition();
    static unsigned long getStartTime();
    static int getPpmSinceVentilation();
    static int getLowest();

  private:
    static void debugMeassure();
    //meassure
    static void meassureAirCondition();
    //data
    static void mapAirCondition();
    static void calculateGradient();
    static void checkVentilating();
    static void setState();

    static float airCondition;
    static float airConditionRaw;
    static float airConditionLast;
    static int airConditionLowest;
    static unsigned long startTime;
    static unsigned long timer;
    static float gradient;
    static int value;
    static int values[AVERAGING_GRADIENT * 2];
    static int now;
    static int last;
    static int ppmSinceVentilation;
    static State state;
    static int colorState;
    static int lowest;

};

#endif
