/*
  TimerGui.h - TimerGui-Library for Airduino.
*/

#ifndef TimerGui_h
#define TimerGui_h

#include "Arduino.h"
#include "../../util/Util.h"
#include "../../manager/Manager.h"

class TimerGui {
  public:
    static void loop();
    static void initGui();
    static void drawTimer();
    static void drawAllArrows(int color);
    static void drawArrowDown(int x, int y, int size, int color);
    static void updateDigit(int digit, int difference);
    static void drawArrowUp(int x, int y, int size, int color);
    static void toggleTimer();
    static void startTimer();
    static void stopTimer();
    static void resetTimer();
    static void drawButtons();
    static void drawDigits(int _millis);
    static void drawIcon();
    static String getDigitsOfMillis(int _millis);
    static void peep();
    static void resetTimerValues();
    static void handleTouch(TSPoint p);

    static int goalMillis;
    static int millisToGoal;
    static boolean isRunning;
    static int millisDifference;
    static String lastTime;
    static String lastDifference;
    static int peepCount;
  private:
    

};

#endif

