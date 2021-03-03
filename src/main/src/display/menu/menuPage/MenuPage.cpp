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

void MenuPage::setup(boolean redraw) {
  _inputOne.setup(redraw);
  _inputTwo.setup(redraw);
  _inputThree.setup(redraw);
  _inputFour.setup(redraw);
  _inputFive.setup(redraw);
  _inputSix.setup(redraw);
}

void MenuPage::handleTouch(TSPoint p) {
  _inputOne.handleTouch(p);
  _inputTwo.handleTouch(p);
  _inputThree.handleTouch(p);
  _inputFour.handleTouch(p);
  _inputFive.handleTouch(p);
  _inputSix.handleTouch(p);
}

void MenuPage::reset(boolean active) {
  _inputOne.reset(active);
  _inputTwo.reset(active);
  _inputThree.reset(active);
  _inputFour.reset(active);
  _inputFive.reset(active);
  _inputSix.reset(active);
}
