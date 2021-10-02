/*
  Display.h - Display-Library for Airduino.
*/

#ifndef Display_h
#define Display_h

#include "Arduino.h"
#include "displayVX/DisplayVX.h"
#include "displayV1/DisplayV1.h"
#include "displayV2/DisplayV2.h"
#include "displayV3/DisplayV3.h"
#include "../guis/stopwatchGui/StopwatchGui.h"
#include "../guis/weatherGui/WeatherGui.h"
#include "../guis/timerGui/TimerGui.h"
#include "../guis/menuGui/MenuGui.h"
#include "../guis/randomStudentGui/RandomStudentGui.h"
#include "../guis/overviewGui/OverviewGui.h"
#include "../guis/decibelGui/DecibelGui.h"
#include "../guis/clockGui/ClockGui.h"
#include "../guis/timeGui/TimeGui.h"



extern const String software_version;


class Display {
  public:
    static void setup();
    
    static void loop();
    static void initDisplay();
    static void handleTouch(); //TODO relocate
    static void drawInfoScreen(int time);
    static void reconnect();
    static void drawTutorial();
    static void drawAuthors();
    static void initAllGuis();
    static void drawTopBar(String title, String oldTitle = "");
    static unsigned long lastTouch;
    static void screenShot();
    

  private:
    static TSPoint p;
    
    static int lastGui;
    
};

#endif
