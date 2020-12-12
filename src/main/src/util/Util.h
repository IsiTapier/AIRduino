/*
  Util.h - Util-Library for Airduino.
*/

#ifndef Util_h
#define Util_h

#include "Arduino.h"
#include "EEPROM.h"
#include "../classes/Pair/Pair.h"
#include "../classes/calibration/Calibration.h"

//pins
#define LED_RED    5
#define LED_GREEN  6
#define LED_BLUE   7

//colors
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define ORANGE 0xFC00
#define WHITE 0xFFFF
#define GREY 0x8C51
#define LIME 0x87F4
#define LIGHT_BLUE 0x76BF
#define TURKISE 0x3FFA

#define PPM_COLOR_N GREEN //Normal
#define PPM_COLOR_R 0xFE60 //Risk
#define PPM_COLOR_A 0xFA27 //Alarm

enum State {VENTILATING = -1, GOOD = 0, MEDIUM = 1, BAD = 2, BLINK = 3, PIEP = 4};

class Util {

  public:
    //State
    static State states[7];
    static int getColor(State state);
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
