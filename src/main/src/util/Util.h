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
#include "../extern/Adafruit_TouchScreen-master/TouchScreen.h"
#include "../extern/TFT_eSPI-master/TFT_eSPI.h"
#include "../util/data/Images.h"
#include "Colors.h"
#include "debug/Debug.h"
#include "../display/Design.h"
#include "settings/Settings.h"
#include "database/Database.h"
#include "../meassure/Meassure.h"

#define LIMIT_GOOD 600
#define LIMIT_MEDIUM 800
#define LIMIT_BAD 900
#define LIMIT_BLINK 1000
#define LIMIT_PIEP 1200

#define EEPROM_SIZE (1+4*sizeof(short))

#define XMIN (1)
#define XMAX (1+1*sizeof(short))
#define YMIN (1+2*sizeof(short))
#define YMAX (1+3*sizeof(short))

//pins

  enum Version {
    V1 = false,
    V2 = true
  };

  enum Mode {
    LOADINGSCREEN = 0,
    CHART = 1,
    MENU = 2
  };

  template<class T>
  struct _init_list_with_square_brackets {
    const std::initializer_list<T>& list;
    _init_list_with_square_brackets(const std::initializer_list<T>& _list): list(_list) {}
    T operator[](unsigned int index) {
      return *(list.begin() + index);
    }
  };

  template<class T>
  _init_list_with_square_brackets<T> _(const std::initializer_list<T>& list) {
    return _init_list_with_square_brackets<T>(list);
  }

  template<class T, size_t size>
  int sizeOf(const T (&array)[size]) {
    return (sizeof(array)/sizeof(T));
  }

  template<class T>
  int sizeOf(T* (&array)) {
    return (sizeof(array)/sizeof(T));
  }

  //global variables
  extern TFT_eSPI display;
  extern TouchScreen ts;
  extern Calibration calibration[];

  //State
  State getStateOf(int value);

  //long map(double x, long in_min, long in_max, long out_min, long out_max);
  int map(int value, int startA, int startB, float factor);
  int average(int averageArray[], int averageStart, int averageEnd);
  void drawBorder(int x, int y, int length, int height, int thickness, int color);
  void drawLine(int x, int y, int length, int height, int color, int a = 0, int b = 0, boolean filter1 = false, int filtervalue1 = 0, boolean filter2 = false, State filtervalue2 = GOOD);
  void showBoxes();
  void dPrint(String text, int x, int y, int size, int color, int datum = 0, int backgroundColor = -1, String oldText = "", int oldTextSize = -1, int padding = 0);
  void dPrint(int text, int x, int y, int size, int color, int datum = 0, int backgroundColor = -1, int oldText = -1, int oldTextSize = -1, int padding = 0);
  void loadingScreen();
  void writeLoadingScreenTitle();
  void writeLoadingAnimation(int c1, int c2, int c3);
  boolean requestDecision(String error, String question, String op1, String op2);
#endif
