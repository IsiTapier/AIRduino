/*
  Manager.h - Manager-Library for Airduino.
*/

#ifndef Manager_h
#define Manager_h

#include "Arduino.h"
#include "display/menu/Menu.h"
#include "util/Util.h"

class Manager {
  public:
    static void setup();
    static void loop();
    static void eeprom();
    /*static unsigned long getLastModeChange();
    static unsigned long getLastLoop();
    static unsigned long getLastReconnect();
    static unsigned long getLastCycleTime();
    static unsigned long getCurrentCycleTime();*/
    static unsigned long lastCycleTime;
    static unsigned long lastLoop;
    static unsigned long lastReconnect;
    static unsigned long lastModeChange;
    static unsigned long currentCycleTime;
  private:
    
};

#include "display/Display.h"
#endif