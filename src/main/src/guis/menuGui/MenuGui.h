/*
  MenuGui.h - MenuGui-Library for Airduino.
*/

#ifndef MenuGui_h
#define MenuGui_h

#include "Arduino.h"
#include "../../util/Util.h"
#include "../../manager/Manager.h"

class MenuGui {
  public:
    static void loop();
    static void initGui();
    static void handleTouch(TSPoint p);
    static void drawTimeDataAtUnderbar();
  private:
    

};

#endif

