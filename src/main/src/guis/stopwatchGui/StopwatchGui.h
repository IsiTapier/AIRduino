/*
  StopwatchGui.h - StopwatchGui-Library for Airduino.
*/

#ifndef StopwatchGui_h
#define StopwatchGui_h

#include "Arduino.h"
#include "../../util/Util.h"
#include "../../manager/Manager.h"

class StopwatchGui {
  public:
    static void loop();
    static void startStopwatch();
    static void stopStopwatch();
    static void resetStopwatch();
    static void toggleStopwatch();
    static void drawStopwatch();
    static void drawLines();
    static void drawDigits(long _millis);
    static void initGui();
    static void drawIcon();
    static void drawButtons();
    

    static String lastTime;
    static int startMillis;
    static boolean isRunning;
    static int millisAtPause;
  private:

};

#endif

