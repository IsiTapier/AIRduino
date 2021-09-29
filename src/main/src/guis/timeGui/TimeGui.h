/*
  TimeGui.h - TimeGui-Library for Airduino.
*/

#ifndef TimeGui_h
#define TimeGui_h

#include "Arduino.h"
#include "../../util/Util.h"
#include "../../manager/Manager.h"

class TimeGui {
  public:
    static void initGui();
    static void handleTouch(TSPoint p);
  private:

    
};

#endif

