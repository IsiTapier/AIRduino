/*
  ClockGui.h - ClockGui-Library for Airduino.
*/

#ifndef ClockGui_h
#define ClockGui_h

#include "Arduino.h"
#include "../../util/Util.h"
#include "../../manager/Manager.h"

class ClockGui {
  public:
    static void initGui();
    static void drawClock(int x, int y, int size, int datum, int color);
  private:
    

};

#endif

