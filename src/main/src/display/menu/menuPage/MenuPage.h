/*
  MenuPage.h - MenuPage-Library for Airduino.
*/

#ifndef MenuPage_h
#define MenuPage_h

#include "Arduino.h"
#include "../../../classes/input/Input.h"

using namespace general;

class MenuPage {
  public:
    MenuPage(Input inputOne = Input(&empty), Input inputTwo = Input(&empty), Input inputThree = Input(&empty), Input inputFour = Input(&empty), Input inputFive = Input(&empty), Input inputSix = Input(&empty));
    void setup(boolean redraw = false);
    void reset(boolean active);
    void handleTouch(TSPoint p);

  private:
    Input _inputOne;
    Input _inputTwo;
    Input _inputThree;
    Input _inputFour;
    Input _inputFive;
    Input _inputSix;
};

#endif
