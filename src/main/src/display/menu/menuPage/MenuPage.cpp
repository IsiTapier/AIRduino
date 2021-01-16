/*
 MenuPage.cpp - MenuPage-Library for Librarys.
*/

#include "MenuPage.h"

extern int _page;
extern Input _inputOne;
extern Input _inputTwo;
extern Input _inputThree;
extern Input _inputFour;
extern Input _inputFive;
extern Input _inputSix;


extern MenuPage::MenuPage(int number, Input inputOne, Input inputTwo, Input inputThree, Input inputFour, Input inputFive, Input inputSix) {
  _page = number;
  _inputOne = inputOne;
  _inputTwo = inputTwo;
  _inputThree = inputThree;
  _inputFour = inputFour;
  _inputFive = inputFive;
  _inputSix = inputSix;
  _inputOne.setup(0);
  _inputTwo.setup(1);
  _inputThree.setup(2);
  _inputFour.setup(3);
  _inputFive.setup(4);
  _inputSix.setup(5);
}

extern void MenuPage::init() {
  if(mode == MENU) {   //TODO page check
    _inputOne.init();
    _inputTwo.init();
    _inputThree.init();
    _inputFour.init();
    _inputFive.init();
    _inputSix.init();
  }
}

extern void MenuPage::handleTouch(TSPoint p) {
  if(mode == MENU) {   //TODO page check
    _inputOne.handleTouch(p);
    _inputTwo.handleTouch(p);
    _inputThree.handleTouch(p);
    _inputFour.handleTouch(p);
    _inputFive.handleTouch(p);
    _inputSix.handleTouch(p);
  }
}
