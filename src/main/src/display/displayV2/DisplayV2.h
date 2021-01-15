/*
  DisplayV2.h - DisplayV2-Library for Airduino.
*/

#ifndef DisplayV2_h
#define DisplayV2_h

#include "../displayVX/Display.h"

class DisplayV2 : Display {

  public:
    static void setup();
    static void loop();

  private:
    static void drawBarBorder();
    static void drawSections();
    static void drawBar();
    static void writeAnalogValue();

    static int barPixel;
    static int lastBarPixel;
};

#endif
