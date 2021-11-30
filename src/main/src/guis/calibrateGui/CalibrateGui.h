/*
  CalibrateGui.h - CalibrateGui-Library for Airduino.
*/

#ifndef CalibrateGui_h
#define CalibrateGui_h

#include "Arduino.h"
#include "../../util/Util.h"
#include "../../manager/Manager.h"

class CalibrateGui {
  public:
    static void loop();
    static void initGui();
    static void handleTouch(TSPoint p);
  private:
    static void drawMaxPPM(int x, int y, int size, int color, int datum);
    static void autoCalibrate();
    static void drawLastCali();
};

#endif

