/*
 Button.cpp - Button-Library for Librarys.
*/

#include "Button.h"

  Button::Button(void) {}

  Button::Button(Setting* setting) {
    _setting = setting;
  }

  Button& Button::setSection(byte section) {
    _section = section;
    return(*this);
  }

  void Button::init() {
    write();
  }

  void Button::checkTouch(TSPoint p) {
    if(p.isTouching(0, MENU_SECTION_LENGTH, MENU_SECTION_Y(_section), (MENU_SECTION_Y(_section) + MENU_SECTION_HEIGHT))) {
      execute();
       // TODO universal
      if(strlen(_setting->getDebugMessage()) != 0)
        debug(DEBUG, MENUD, _setting->getDebugMessage());
    }
  }

  void Button::write() {
    String text = _setting->getTitle();
    dPrint(text, MENU_SECTION_LENGTH/2, MENU_SECTION_TEXT_Y(_section), MENU_SECTION_SIZE, TEXT_COLOR, 4);
  }
  
  void Button::execute() {
    switch(_setting->getDefaultValue()) {
      case 0: ts.calibration(); break;
      case 1: Meassure::calibrateMin(); break;
      case 2: Meassure::calibrateMax(); break;
      case 3: Meassure::autoCalibrationMin(); break;
      default: break;
    }

  }

