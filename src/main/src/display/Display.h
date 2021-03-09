/*
  Display.h - Display-Library for Airduino.
*/

#ifndef Display_h
#define Display_h

#include "Arduino.h"
#include "displayVX/DisplayVX.h"
#include "displayV1/DisplayV1.h"
#include "displayV2/DisplayV2.h"
#include "menu/Menu.h"
#include "../util/Util.h"

class Display {
  public:
    static void setup();
    static void eeprom();
    static void loop();
    static void initDisplay();
    static void handleTouch(); //TODO relocate

  private:
    static TSPoint p;
    static unsigned long lastModeChange;
    static int counter;
};

#endif
