/*
  Display.h - Display-Library for Airduino.
*/

#ifndef Display_h
#define Display_h

#include "Arduino.h"
#include "Wire.h"
//#include "../../Adafruit/Adafruit_TouchScreen-master/TouchScreen.h"
#include "../../extern/TFT_eSPI-master/TFT_eSPI.h"
#include "../../util/Util.h"
#include "../../meassure/Meassure.h"
#include "Design.h"

class Display {

  public:
    static void setup();

  protected:
    static void drawDisplay();
    static void getData();
    static void generateData(int startPPM, int endPPM, int changePPM);
    static void drawLoadingBar();
    static void checkState();
    static void writeInfo();
    static void drawBorder(int x, int y, int length, int height, int thickness, int color);
    static void drawLine(int x, int y, int length, int height, int color, int a = 0, int b = 0, boolean filter1 = false, boolean filter2 = false);
    //static void createLines();
    static void showBoxes();
    static void dPrint(String text, int x, int y, int scale, int color, int datum = 0, int backgroundColor = -1, String oldText = "", int padding = 0);
    static void dPrint(int text, int x, int y, int scale, int color, int datum = 0, int backgroundColor = -1, int oldText = -1, int padding = 0);

  //  static Adafruit_TFTLCD display;
    static TFT_eSPI display;
    //static TouchScreen ts;

    static State state;
    static State lastState;
    static int airCondition;
    static int lastAirCondition;
    static boolean blinkSwitch;
    static int statusLetters;
    static String statusInfo;
    static String lastTime;
    static String time;
    static short seconds;
    static short minutes;
    static boolean start;
    static boolean drop;

    static void loadingScreen();
  private:
    static void writeLoadingScreenTitle();

};

#endif
