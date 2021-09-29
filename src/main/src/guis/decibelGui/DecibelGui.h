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
    static void registerTouch(TSPoint ts);
    static void drawTrafficLight(int value, int x, int y, int size);
    static int trafficLightStage;
    static boolean isActive;

  private:
    static void drawSlider(int sliderX, int x, int y, int length, int height);
    static void drawBar(int value, int color);
    static void drawStripes(int value, int color);
    static void drawSliderFactor(int value);
    
};

#endif

