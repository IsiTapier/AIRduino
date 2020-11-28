/*
  DisplayV2.h - DisplayV2-Library for Airduino.
*/

#ifndef DisplayV2_h
#define DisplayV2_h

#include "../displayVX/Display.h"

#define BORDER_X 10
#define BORDER_Y 43
#define BORDER_WIDTH DISPLAY_LENGTH - BORDER_X * 2
#define BORDER_HIGHT 40

#define BAR_X BORDER_X + 2
#define BAR_Y BORDER_Y + 2
#define BAR_WIDTH BORDER_WIDTH - (2 * 2)
#define BAR_HIGHT BORDER_HIGHT - (2 * 2)

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
    static short first_section_x;
    static short second_section_x;

};

#endif
