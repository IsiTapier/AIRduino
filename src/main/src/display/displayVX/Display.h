/*
  Display.h - Display-Library for Airduino.
*/

#ifndef Display_h
#define Display_h

#include "Arduino.h"
#include "Wire.h"
#include "../../Adafruit/Adafruit_TouchScreen-master/TouchScreen.h"
#include "../../Adafruit/Adafruit_GFX_Library-1.5.3/Adafruit_GFX.h"
#include "../../Adafruit/TFTLCD-Library-master/Adafruit_TFTLCD.h"
#include "../../util/Util.h"
#include "../../meassure/Meassure.h"

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

//settings
#define ROTATION 45
#define DISPLAY_BRIGHTNESS 0.7
#define LOADING_SCREEN_TIME 1

#define AVERAGING_GRAPH 10
#define ALPHA_GRAPH 0.7

//pins:
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

#define TS_MINX 920
#define TS_MINY 120
#define TS_MAXX 150
#define TS_MAXY 940


#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define PIEZO 4

#define DISPLAY_LENGTH 319
#define DISPLAY_WIDTH 239

//Design
#define MAX_LIGHT 1000
#define OSV_PPM 400
#define MAX_DISPLAYED_PPM 1280

#define STATUS_INFO_TOP_MARGIN 5
#define DATABOX_TOP_HIGHT 400
#define GRAPH_COLOR WHITE//MAGENTA
#define GRAPH_BACKGROUND_COLOR BLACK
#define BAR_BACKGROUND_COLOR 0x2104 //Fast schwarz
#define BAR_STRIPE_THICKNESS 10
#define TIME_COLOR_CRITICAL RED
#define TIME_COLOR_NORMAL WHITE

#define PPM_DIGIT_SIZE 6
#define PPM_DIGIT_MARGIN 5 //Rand links

#define PPM_STRING_SIZE 4
#define PPM_STRING_MARGIN 5 // Rand von unten

#define TIMER_SIZE 4


//loadingScreen
#define LOADING_SCREEN_TITLE_SIZE 12
#define LOADING_SCREEN_SUB_SIZE 8
#define LOADING_SCREEN_DOTS_COLOR WHITE
#define LOADING_SCREEN_DOT_SIZE 10


class Display {

  public:
    static void setup();

  protected:
    static void drawDisplay();
    static void drawLoadingBar();
    static void checkState();
    static void writeInfo();
    static void drawBorder(int xStart, int yStart, int xEnd, int yEnd, int color);
    static void drawLine(int x, int y, int z);
    static void createLines();
    static void dPrint(int x, int y, int scale, int color, String text);
    static void dPrint(int x, int y, int scale, int color, int text);

  //  static Adafruit_TFTLCD display;
    static Adafruit_TFTLCD display;
    static TouchScreen ts;
    static uint16_t identifier;

    static State state;
    static State previousState;
    static int airCondition;
    static int lastAirConditionGraph;
    static boolean blinkSwitch;
    static int statusLetters;
    static String statusInfo;
    static String lastTime;
    static String Time;
    static short seconds;
    static short minutes;

    static void loadingScreen();
  private:
    static void writeLoadingScreenTitle();

};

#endif
