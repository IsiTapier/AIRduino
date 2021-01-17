/*
  MenuPage.h - MenuPage-Library for Airduino.
*/

#ifndef MenuPage_h
#define MenuPage_h

#include "Arduino.h"
#include "../../../classes/input/Input.h"

class MenuPage {
  public:
    MenuPage(Input inputOne = Input(EMPTY), Input inputTwo = Input(EMPTY), Input inputThree = Input(EMPTY), Input inputFour = Input(EMPTY), Input inputFive = Input(EMPTY), Input inputSix = Input(EMPTY));
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
