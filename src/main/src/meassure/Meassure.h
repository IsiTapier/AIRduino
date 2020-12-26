/*
  Meassure.h - Meassure-Library for Airduino.
*/

#ifndef Meassure_h
#define Meassure_h

#include "Arduino.h"
#include "../util/Util.h"

//settings
#define DEBUG false

#define FACTOR 4.631578947368421
#define STAGE_TIME 1*1000
#define VENTILATING_TIMEOUT 5*1000

#define AVERAGING_MEASUREMENTS 100
#define AVERAGING_GRADIENT 10
#define ALPHA_MEASUREMENTS 0.7
#define ALPHA_LOWEST 0.5


#define MAX_SENSOR 1023
#define MAX_PPM 5000

#define MAX_INCREASE 1.5
#define MAX_DECREASE 0.9
#define MAX_INCREASE_LOWEST 50




//pins
#define GAS_SENSOR 36
#define PIEZO      18


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
    static void debug();
    //meassure
    static void meassureAirCondition();
    //data
    static void mapAirCondition();
    static void calculateGradient();
    static void checkVentilating();
    static void setState();
    static void writeLed();

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
    static int led;
    static int lowest;
    static int red;
    static int green;
    static int blue;

};

#endif
