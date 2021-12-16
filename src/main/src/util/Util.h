/*
  Util.h - Util-Library for Airduino.
*/

#ifndef Util_h
#define Util_h

#include "Arduino.h"
#include "EEPROM.h"
#include <pgmspace.h>
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

#define EEPROM_SIZE (1+7*sizeof(short) + sizeof(bool))
#define ALLOW_PIEP false

#define XMIN (1)
#define XMAX (1+1*sizeof(short))
#define YMIN (1+2*sizeof(short))
#define YMAX (1+3*sizeof(short))
#define MAP_MAX_IN (1+4*sizeof(short))
#define MAP_MAX_OUT (1+5*sizeof(short))
#define EEPROM_SENSOR_ID (1+6*sizeof(short))
#define MAP_IS_ACTIVE (1+7*sizeof(short))

#define GET_MAP_IS_ACTIVE (EEPROM.readBool(MAP_IS_ACTIVE))
#define GET_MAP_MAX_IN (EEPROM.readShort(MAP_MAX_IN))
#define GET_MAP_MAX_OUT (EEPROM.readShort(MAP_MAX_OUT))
#define GET_EEPROM_SENSOR_ID (EEPROM.readShort(EEPROM_SENSOR_ID))

#define SET_MAP_IS_ACTIVE(value) EEPROM.writeBool(MAP_IS_ACTIVE, value); EEPROM.commit()
#define SET_MAP_MAX_IN(value) EEPROM.writeShort(MAP_MAX_IN, value); EEPROM.commit()
#define SET_MAP_MAX_OUT(value) EEPROM.writeShort(MAP_MAX_OUT, value); EEPROM.commit()
#define SET_EEPROM_SENSOR_ID(value) Serial.println(value); EEPROM.writeShort(EEPROM_SENSOR_ID, value); EEPROM.commit()



//pins

  enum Version {
    V1 = 1,
    V2 = 2,
    V3 = 3
  };

  enum Mode {
    LOADINGSCREEN = 0,
    CHART = 1,
    MENU = 2,
    MAINTENANCE = 3,
    RESET = 4
  };

  enum GUIS {
    CO2_GUI = 0,
    STOPWATCH_GUI = 1,
    WEATHER_GUI = 2,
    TIMER_GUI = 3,
    RANDOM_STUDENT_GUI = 4,
    DECIBEL_GUI = 5,
    GUI_MENU = 6,
    OVERVIEW_GUI = 7,
    TIME_GUI = 8,
    CLOCK_GUI = 9,
    CALIBRATE_GUI = 10
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

  template<class T>
  bool contains(std::vector<T> vector, T value) {
    for(int i = 0; i < vector.size(); i++)
      if(vector[i] == value)
        return true;
    return false;
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
  void dPrint(String text, int x, int y, int size, int color, int datum = 0, int backgroundColor = -1, String oldText = "", int oldTextSize = -1, boolean redraw = false, int padding = 0);
  void dPrint(int text, int x, int y, int size, int color, int datum = 0, int backgroundColor = -1, int oldText = -1, int oldTextSize = -1, boolean redraw = false, int padding = 0);
  void loadingScreen();
  void writeLoadingScreenTitle();
  void writeLoadingAnimation(int c1, int c2, int c3);
  boolean requestDecision(String topic, String question, String op1 = "ja", String op2 = "nein", long timeout = REQUESTDECISIONTIMEOUT, bool defaultValue = false);
  void drawLogo();
  boolean haveEssentialsChanged();
#endif
