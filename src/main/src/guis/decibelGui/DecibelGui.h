/*
  DecibelGui.h - DecibelGui-Library for Airduino.
*/

#ifndef DecibelGui_h
#define DecibelGui_h

#include "Arduino.h"
#include "../../util/Util.h"
#include "../../manager/Manager.h"

class DecibelGui {
  public:
    static void loop();
    static void initGui();
    static void setup();

  private:
    static void drawBar(int value, int color);
    static void drawStripes(int value, int color);
};

#endif

