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
#define GAS_SENSOR 36
#define PIEZO      21

  enum Version {
    V1 = false,
    V2 = true
  };

  enum Mode {
    LOADINGSCREEN = 0,
    CHART = 1,
    MENU = 2
  };

  /*enum InputType {
    SLIDER,
    EMPTY
  };*/

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

  extern TFT_eSPI display;
  extern TouchScreen ts;
  extern Calibration calibration[];
  extern Mode mode;
  extern Mode lastMode;
  extern Version lastVersion;

  //Database connection
  extern unsigned long lastMsg;
  #define MSG_BUFFER_SIZE  (250)
  extern char msg[MSG_BUFFER_SIZE];
  extern int value;
  extern String device_grade;

  extern const char* ssid;
  extern const char* password;
  extern const char* mqtt_server;

  void setupDatabaseConnection();
  void callback(char* topic_char, byte* payload, unsigned int length);
  void config_request();
  void subToConfigChannel();
  void subscribeToActivityRequest();
  void config_update(String column, String value);
  void mysql_insert(String grade, int co2, double temp, double humidity, double pressure, double altitude);
  void getUniqueID();
  void setup_wifi();
  void reconnect();

#endif
