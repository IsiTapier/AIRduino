/*
  Menu.h - Menu-Library for Airduino.
*/

#ifndef Menu_h
#define Menu_h

#include "Arduino.h"
#include "../../util/Util.h"
#include "../../classes/slider/Slider.h"

class Menu {
  public:
    static void setup();
    static void loop();
    static void checkTouch(TSPoint p);

  private:
    static void drawDisplay();

    static Slider firstSection;
    static Slider secondSection;
    static Slider thirdSection;
    static Slider fourthSection;
    static Slider fifthSection;
    static Slider sixthSection;
};

#endif
