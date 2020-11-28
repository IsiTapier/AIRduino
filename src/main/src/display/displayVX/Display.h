/*
  Display.h - Display-Library for Airduino.
*/

#ifndef Display_h
#define Display_h

#include "Arduino.h"
#include "Wire.h"
#include "../../Adafruit/Adafruit_GFX_Library/Adafruit_GFX.h"
#include "../../Adafruit/Adafruit_ST7735_and_ST7789_Library/Adafruit_ST7735.h"
#include "../../util/Util.h"
#include "../../meassure/Meassure.h"

//settings
#define ROTATION 45
#define DISPLAY_BRIGHTNESS 0.7
#define DEBUG false
#define LOADING_SCREEN_TIME 2

#define AVERAGING_GRAPH 10
#define ALPHA_GRAPH 0.7

//pins:
#define TFT_CS     8
#define TFT_RST    10
#define TFT_DC     9
#define TFT_LED    3
#define PIEZO      4

//Data:
#define DISPLAY_LENGTH 159
#define DISPLAY_WIDTH 127

//Design
#define MAX_LIGHT 1000
#define OSV_PPM 400
#define MAX_DISPLAYED_PPM 1280

#define LIMIT_GOOD 600
#define LIMIT_MEDIUM 800
#define LIMIT_BAD 900
#define LIMIT_ALARM 1000
#define LIMIT_PIEP 1200

#define DATABOX_TOP_HIGHT 100
#define GRAPH_COLOR WHITE//MAGENTA
#define GRAPH_BACKGROUND_COLOR BLACK
#define BAR_BACKGROUND_COLOR 0x2104 //Fast schwarz
#define BAR_STRIPE_THICKNESS 3
#define TIME_COLOR_CRITICAL RED
#define TIME_COLOR_NORMAL WHITE
#define LOADING_SCREEN_DOTS_COLOR WHITE


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

    static Adafruit_ST7735 display;
    static State state;
    static State previousState;
    static int airCondition;
    static int lastAirConditionGraph;
    static boolean blinkSwitch;
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
