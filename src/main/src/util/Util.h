/*
  Util.h - Util-Library for Airduino.
*/

#ifndef Util_h
#define Util_h

#include "Arduino.h"
#include "EEPROM.h"
#include <pgmspace.h>
#include "../classes/Pair/Pair.h"
#include "../classes/calibration/Calibration.h"
#include "../classes/state/State.h"
#include "Colors.h"

//pins
#define LED_RED    11
#define LED_GREEN  12
#define LED_BLUE   13

#define LIMIT_GOOD 600
#define LIMIT_MEDIUM 800
#define LIMIT_BAD 900
#define LIMIT_BLINK 1000
#define LIMIT_PIEP 1200


class Util {

  public:
    //State
    static State getStateOf(int value);

    static Calibration calibration[];

    static void setup();
    static void loop();
    static long map(double x, long in_min, long in_max, long out_min, long out_max);
    static int map(int value, int startA, int startB, float factor);
    static void rgb(int red, int green, int blue);
    static int average(int averageArray[], int averageStart, int averageEnd);
    static void debug(String title, float value);

  private:

};

#endif
