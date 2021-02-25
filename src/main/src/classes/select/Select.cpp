/*
 Select.cpp - Select-Library for Librarys.
*/

#include "Select.h"

using namespace general;

  Select::Select(void) {}

  Select::Select(Setting* setting) {
    _setting = setting;
  }

  Select& Select::setSection(byte section) {
    _section = section;
    return(*this);
  }

  byte Select::getSection() {
    return(_section);
  }

  void Select::init() {
    draw();
  }

  void Select::checkTouch(TSPoint p) {
    if(p.isTouching(0, MENU_SELECT_LENGTH, MENU_SECTION_Y(_section), MENU_SECTION_END_Y(_section))) {
      setValue(false);
    } else if(p.isTouching(MENU_SELECT_LENGTH, MENU_SELECT_LENGTH*2, MENU_SECTION_Y(_section), MENU_SECTION_END_Y(_section))) {
      setValue(true);
    }
    if(_setting->getDebugMessage() != "")
      debug(DEBUG, MENUD, _setting->getDebugMessage());
  }

  void Select::setValue(boolean value) {
    if(_setting->getValue() != value) {
      _setting->setValue(value);
      if(debugMenu.getValue() && debugPriority.getValue())
        debug(INFO, MENUD, "Value changed to", _setting->getValue());
    }
  }

  void Select::draw() {
    display.drawLine(MENU_SELECT_LENGTH, MENU_SECTION_Y(_section), MENU_SELECT_LENGTH, MENU_SECTION_END_Y(_section), TEXT_COLOR);
    write();
  }

  void Select::write() {
    String text = _setting->getName(_setting->getValue());
    dPrint(text, round(MENU_SECTION_LENGTH/4), MENU_SECTION_TEXT_Y(_section), MENU_SECTION_SIZE, TEXT_COLOR, 4);
    text = _setting->getName(!_setting->getValue());
    dPrint(text, floor(MENU_SECTION_LENGTH/4*3), MENU_SECTION_TEXT_Y(_section), MENU_SECTION_SIZE, TEXT_COLOR, 4);
  }

  void Select::reset() {
    if(_setting->getValue() != _setting->getDefaultValue())
      setValue(_setting->getDefaultValue());
  }
