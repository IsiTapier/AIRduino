/*
 Number.cpp - Number-Library for Librarys.
*/

#include "Number.h"

  Number::Number(void) {}

  Number::Number(Setting* setting) {
    _setting = setting;
  }

  Number& Number::setSection(byte section) {
    _section = section;
    return(*this);
  }

  byte Number::getSection() {
    return(_section);
  }

  void Number::init() {
    if(general::language.hasChanged())
      write(true);
    draw();
  }

  void Number::checkTouch(TSPoint p) {
    if(p.isTouching(MENU_NUMBER_PLUS_START_X, MENU_NUMBER_PLUS_END_X, MENU_INPUT_ICON_START_Y(_section), MENU_INPUT_ICON_END_Y(_section)))
      setValue(_setting->getValue()+1);
    else if(p.isTouching(MENU_NUMBER_MINUS_START_X, MENU_NUMBER_MINUS_END_X, MENU_INPUT_ICON_START_Y(_section), MENU_INPUT_ICON_END_Y(_section)))
      setValue(_setting->getValue()-1);
    else
      return;
    if(strlen(_setting->getDebugMessage()) != 0)
      debug(DEBUG, MENUD, _setting->getDebugMessage());
  }

  void Number::setValue(short value, boolean active) {
    if(value > _setting->getMaxValue())
      value = _setting->getMinValue();
    if(value < _setting->getMinValue())
      value = _setting->getMaxValue();
    _setting->setValue(value);
    if(active)
      write();
    debug(INFO, MENUD, "Value changed to", _setting->getValue());
  }

  void Number::draw() {
    if(general::theme.getValue()) {
      display.pushImage(MENU_NUMBER_PLUS_START_X, MENU_INPUT_ICON_START_Y(_section), MENU_INPUT_ICON_LENGTH, MENU_INPUT_ICON_HEIGHT, plusDark, BLACK);
      display.pushImage(MENU_NUMBER_MINUS_START_X, MENU_INPUT_ICON_START_Y(_section), MENU_INPUT_ICON_LENGTH, MENU_INPUT_ICON_HEIGHT, minusDark, BLACK);
    } else {
      display.pushImage(MENU_NUMBER_PLUS_START_X, MENU_INPUT_ICON_START_Y(_section), MENU_INPUT_ICON_LENGTH, MENU_INPUT_ICON_HEIGHT, plusLight, WHITE);
      display.pushImage(MENU_NUMBER_MINUS_START_X, MENU_INPUT_ICON_START_Y(_section), MENU_INPUT_ICON_LENGTH, MENU_INPUT_ICON_HEIGHT, minusLight, WHITE);
    }
    write(true);
  }

  void Number::write(boolean init) {
    String text;
    String oldText;
    if(init) {
      text = _setting->getName();
      if(general::language.hasChanged())
        oldText = _setting->getName(true);
      else
        oldText = " ";
      dPrint(text, MENU_MARGIN_LEFT, MENU_SECTION_TEXT_Y(_section), MENU_NUMBER_TITLE_SIZE, TEXT_COLOR, 3, BACKGROUND_COLOR, oldText, MENU_NUMBER_OLD_TITLE_SIZE);
      oldText = "";
    } else
      oldText = String(_setting->getOldValue());
    text = String(_setting->getValue());
    dPrint(text, MENU_NUMBER_NUMBER_START_X, MENU_SECTION_TEXT_Y(_section), MENU_NUMBER_NUMBER_SIZE, TEXT_COLOR, 3, BACKGROUND_COLOR, oldText);
  }

  void Number::reset(boolean active) {
    if(_setting->getValue() != _setting->getDefaultValue())
      setValue(_setting->getDefaultValue(), active);
  }
