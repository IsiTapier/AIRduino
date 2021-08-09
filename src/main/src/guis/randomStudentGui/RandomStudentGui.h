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
    static void drawArrows();
    static void drawRandomNumber();
    static void drawMaxRandomValue();
    static void changeMax(int difference);
    static int maxRandomValue;
    static int lastMaxRandomValue;
    static int lastRandomValue;
  private:

};

#endif

