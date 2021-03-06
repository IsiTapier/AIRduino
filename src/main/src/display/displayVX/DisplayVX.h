/*
  Display.h - Display-Library for Airduino.
*/

#ifndef DisplayVX_h
#define DisplayVX_h

#include "Arduino.h"
#include "Wire.h"
#include "../../util/Util.h"

#define ERROR (airCondition <= 0)
#define ERRORLAST (lastAirCondition <= 0)

class DisplayVX {

  public:
    static void setup();
    static void handleData();

  protected:
    static void drawDisplay();
    static void getData();
    static void generateData(int startPPM, int endPPM, int changePPM);
    static void checkState();
    static void writeInfo();
    static void drawEssentials();
    static void drawPPM();
    static String getLastPPMString();
    static String getPPMString(String lastppm);
    static void drawTimer();
    //static void createLines();

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
    static short peep;
    static short peepCounter;

};

#endif
