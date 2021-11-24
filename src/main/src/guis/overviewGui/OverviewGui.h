/*
  OverviewGui.h - OverviewGui-Library for Airduino.
*/

#ifndef OverviewGui_h
#define OverviewGui_h

#include "Arduino.h"
#include "../../util/Util.h"
#include "../../manager/Manager.h"
#include "../weatherGui/WeatherGui.h"
#include "../../display/displayVX/DisplayVX.h"

class OverviewGui {
  public:
    static void loop();
    static void initGui();
    static void drawWeatherData(String temp, String weather, String humidity);
  private:
    static void drawPPM();
    static void drawPPM(boolean atStart);
    
    static String lastPPMString;
    static void drawPPMTimer(int x, int y, int size, int color);
};

#endif

