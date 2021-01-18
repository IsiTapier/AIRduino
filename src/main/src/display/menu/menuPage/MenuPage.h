/*
  MenuPage.h - MenuPage-Library for Airduino.
*/

#ifndef MenuPage_h
#define MenuPage_h

#include "Arduino.h"
#include "../../../classes/input/Input.h"

class MenuPage {
  public:
    MenuPage(Input inputOne = Input(Setting(EMPTY)), Input inputTwo = Input(Setting(EMPTY)), Input inputThree = Input(Setting(EMPTY)), Input inputFour = Input(Setting(EMPTY)), Input inputFive = Input(Setting(EMPTY)), Input inputSix = Input(Setting(EMPTY)));
    void setup();
    void reset();
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
