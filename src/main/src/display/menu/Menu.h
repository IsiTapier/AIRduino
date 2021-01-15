/*
  Menu.h - Menu-Library for Airduino.
*/

#ifndef Menu_h
#define Menu_h

#include "Arduino.h"
#include "../../util/Util.h"

class Menu {
  public:
    static void setup();
    static void loop();

  private:
    static void drawDisplay();
    static void drawFirstSection();
    static void drawSecondSection();
    static void drawThirdSection();
};

#endif
