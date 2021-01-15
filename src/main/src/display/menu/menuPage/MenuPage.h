/*
  MenuPage.h - MenuPage-Library for Airduino.
*/

#ifndef MenuPage_h
#define MenuPage_h

#include "Arduino.h"
#include "../../../classes/slider/Slider.h"

class MenuPage {
  public:
    MenuPage(int number); //TODO types
    void drawPage();
    void checkTouch(TSPoint p);

  private:
    void init();
    void drawSide();

    int page;


};

#endif
