/*
  RandomStudentGui.h - RandomStudentGui-Library for Airduino.
*/

#ifndef RandomStudentGui_h
#define RandomStudentGui_h

#include "Arduino.h"
#include "../../util/Util.h"
#include "../../manager/Manager.h"

class RandomStudentGui {
  public:
    static void initGui();
    static void drawRandomNumber();
    static void changeMax(int difference);
    static void handleTouch(TSPoint p);
    static String shortenName(String x);

  private:
    static void drawGui(bool mode = false);  
    static void drawMaxRandomValue();   
    static void drawSideBar();

    static int minRandomValue;
    static int maxRandomValue;
    static int lastMaxRandomValue;
    static int lastRandomValue;
  

};

#endif

