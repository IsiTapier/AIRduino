/*
  Util.h - Util-Library for Airduino.
*/

#ifndef Util_h
#define Util_h

#include "Arduino.h"
#include "EEPROM.h"
#include "pgmspace.h"
#include "../classes/Pair/Pair.h"
#include "../classes/calibration/Calibration.h"
#include "../classes/state/State.h"
#include "../extern/Adafruit_TouchScreen-master/TouchScreen.h"
#include "../extern/TFT_eSPI-master/TFT_eSPI.h"
#include "../util/data/Images.h"
#include "Colors.h"
#include "../display/Design.h"

#define LIMIT_GOOD 600
#define LIMIT_MEDIUM 800
#define LIMIT_BAD 900
#define LIMIT_BLINK 1000
#define LIMIT_PIEP 1200

//pins
#define GAS_SENSOR 36
#define PIEZO      21

  enum Version {
    V1 = 1,
    V2 = 2
  };

  enum Mode {
    LOADINGSCREEN = 0,
    CHART = 1,
    MENU = 2
  };

  //State
  State getStateOf(int value);

  //long map(double x, long in_min, long in_max, long out_min, long out_max);
  int map(int value, int startA, int startB, float factor);
  int average(int averageArray[], int averageStart, int averageEnd);
  void debug(String title, float value);
  void drawBorder(int x, int y, int length, int height, int thickness, int color);
  void drawLine(int x, int y, int length, int height, int color, int a = 0, int b = 0, boolean filter1 = false, int filtervalue1 = 0, boolean filter2 = false, State filtervalue2 = GOOD);
  void showBoxes();
  void dPrint(String text, int x, int y, int scale, int color, int datum = 0, int backgroundColor = -1, String oldText = "", int padding = 0);
  void dPrint(int text, int x, int y, int scale, int color, int datum = 0, int backgroundColor = -1, int oldText = -1, int padding = 0);
  void loadingScreen();
  void writeLoadingScreenTitle();
  void writeLoadingAnimation(int c1, int c2, int c3);

  extern TFT_eSPI display;
  extern TouchScreen ts;
  extern Calibration calibration[];
  extern Mode mode;
  extern Version version;
  extern Mode lastMode;
  extern Version lastVersion;

#endif
