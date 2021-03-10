/*
  DisplayV3.h - DisplayV3-Library for Airduino.
*/

#ifndef DisplayV3_h
#define DisplayV3_h

#include "../displayVX/DisplayVX.h"

class DisplayV3 : DisplayVX {
  public:
    static void setup();
    static void loop();

  private:
    static void drawDisplay();
    static void writeInfoV3();
    static int stateBackgroundColor;
};

#endif
