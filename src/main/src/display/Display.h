/*
  Display.h - Display-Library for Airduino.
*/

#ifndef Display_h
#define Display_h

#include "Arduino.h"
#include "displayVX/DisplayVX.h"
#include "displayV1/DisplayV1.h"
#include "displayV2/DisplayV2.h"
#include "displayV3/DisplayV3.h"
#include "menu/Menu.h"
#include "../util/Util.h"

extern const String software_version;

#define CURRENTCYCLE floor((currentCycleTime-Meassure::getStartTime())/STAGE_TIME)
#define LASTCYCLE floor((lastCycleTime-Meassure::getStartTime())/STAGE_TIME)

class Display {
  public:
    static void setup();
    static void eeprom();
    static void loop();
    static void initDisplay();
    static void handleTouch(); //TODO relocate
    static void drawInfoScreen(int time);

  private:
    static TSPoint p;
    static unsigned long lastModeChange;
    static unsigned long lastTouch;
    static unsigned long lastCycleTime;
    static unsigned long lastLoop;
};

#endif
