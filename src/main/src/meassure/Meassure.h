/*
  Meassure.h - Meassure-Library for Airduino.
*/

#ifndef Meassure_h
#define Meassure_h

#include "Arduino.h"
#include "../extern/MQ-135/MQ135.h"
#include "../util/Util.h"
#include "../util/settings/Settings.h"
#include "../extern/Adafruit_BME280_Library-master/Adafruit_BME280.h"
#include "../extern/MH_Z19/src/MHZ19.h"

#define RANGE 2000
#define SENSORCONNECTED (MHZ19b.errorCode == RESULT_OK)

class Meassure {

  public:
    //setup
    static void setup();
    static void loop();
    static State getState();
    static int getAirCondition();
    static unsigned long getStartTime();
    static void resetStartTime(boolean cycle = false);
    static int getLowest();
    static int getHighest();
    static void calibrateMin();
    static void calibrateMax();

  private:
    static void debugMeassure();
    //meassure
    static boolean meassureAirCondition();
    static void meassureEnvironment();
    //data
    static void calculateGradient();
    static void checkVentilating();
    static void setState();

    static Adafruit_BME280 bme;
    static MHZ19 MHZ19b;
    static unsigned long tempAirCondition;
    static unsigned long temptempAirCondition;
    static float airConditionTemp;
    static float airCondition;
    static float airConditionRaw;
    static float airConditionLast;
    static unsigned long startTime;
    static unsigned long timer;
    static int gradient;
    static int value;
    static int values[AVERAGING_GRADIENT * 2];
    static int counter;
    static int now;
    static int last;
    static int minPPM;
    static int maxPPM;
    static State state;
    static int colorState;
    static int temperature;
    static int humidity;
    static int pressure;
   
    static int testCounter;
    static unsigned long lasttime;

};

#endif
