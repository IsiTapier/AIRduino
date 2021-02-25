/*
 MenuPage.cpp - MenuPage-Library for Librarys.
*/

#include "MenuPage.h"

using namespace general;

MenuPage::MenuPage(Input inputOne, Input inputTwo, Input inputThree, Input inputFour, Input inputFive, Input inputSix) {
  _inputOne = inputOne;
  _inputTwo = inputTwo;
  _inputThree = inputThree;
  _inputFour = inputFour;
  _inputFive = inputFive;
  _inputSix = inputSix;
  _inputOne.setSection(0);
  _inputTwo.setSection(1);
  _inputThree.setSection(2);
  _inputFour.setSection(3);
  _inputFive.setSection(4);
  _inputSix.setSection(5);
}

void MenuPage::setup() {
  if(mode.equals(MENU)) {   //TODO page check
    _inputOne.setup();
    _inputTwo.setup();
    _inputThree.setup();
    _inputFour.setup();
    _inputFive.setup();
    _inputSix.setup();
  }
}

void MenuPage::handleTouch(TSPoint p) {
  if(mode.equals(MENU)) {   //TODO page check
    _inputOne.handleTouch(p);
    _inputTwo.handleTouch(p);
    _inputThree.handleTouch(p);
    _inputFour.handleTouch(p);
    _inputFive.handleTouch(p);
    _inputSix.handleTouch(p);
  }
}

void MenuPage::reset() {
  _inputOne.reset();
  _inputTwo.reset();
  _inputThree.reset();
  _inputFour.reset();
  _inputFive.reset();
  _inputSix.reset();
}
