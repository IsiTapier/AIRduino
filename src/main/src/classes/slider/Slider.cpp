/*
 Slider.cpp - Slider-Library for Librarys.
*/

#include "Slider.h"

using namespace general;

  Slider::Slider(void) {}

  Slider::Slider(Setting* setting) {
    _setting = setting;
  }

  Slider& Slider::setSection(byte section) {
    _section = section;
    return(*this);
  }

  byte Slider::getSection() {
    return(_section);
  }

  void Slider::init() {
    draw();
    write(true);
  }

  void Slider::checkTouch(TSPoint p) {
    if(p.isTouching(MENU_SLIDER_START_X, MENU_SLIDER_END_X, MENU_SLIDER_START_Y(_section), MENU_SLIDER_END_Y(_section))) {
      changeValue();
      if(_setting->getDebugMessage() != "")
        debug(DEBUG, MENUD, _setting->getDebugMessage());
    }
  }

  void Slider::setValue(boolean value) {
    _setting->setValue(value);
    if(debugMenu.getValue())
      debug(INFO, MENUD, "Value changed to", _setting->getValue());
    if(mode.equals(MENU))
      draw();
  }

  void Slider::changeValue() {
    setValue(!_setting->getValue());
  }

  void Slider::draw() {
    display.fillCircle(MENU_SLIDER_DOT_X(!_setting->getValue()), MENU_SLIDER_DOT_Y(_section), MENU_SLIDER_DOT_RADIUS, BACKGROUND_COLOR);
    display.fillRoundRect(MENU_SLIDER_START_X, MENU_SLIDER_START_Y(_section), MENU_SLIDER_LENGTH, MENU_SLIDER_HEIGHT, MENU_SLIDER_RADIUS, MENU_SLIDER_STRIP_COLOR(_setting->getValue(), _setting->getColored()));
    display.drawRoundRect(MENU_SLIDER_START_X, MENU_SLIDER_START_Y(_section), MENU_SLIDER_LENGTH, MENU_SLIDER_HEIGHT, MENU_SLIDER_RADIUS, TEXT_COLOR);
    display.fillCircle(MENU_SLIDER_DOT_X(_setting->getValue()), MENU_SLIDER_DOT_Y(_section), MENU_SLIDER_DOT_RADIUS, MENU_SLIDER_DOT_COLOR(_setting->getValue(), _setting->getColored()));
    if(_setting->getColored())
      display.drawCircle(MENU_SLIDER_DOT_X(_setting->getValue()), MENU_SLIDER_DOT_Y(_section), MENU_SLIDER_DOT_RADIUS, TEXT_COLOR);
    write();
  }

  void Slider::write(boolean init) {
    String text = "";
    String oldText = "";
    text = _setting->getName();
    if(!init)
      oldText = _setting->getName(true);
    dPrint(text, MENU_MARGIN_LEFT, MENU_SECTION_TEXT_Y(_section), MENU_SECTION_SIZE, TEXT_COLOR, 3, BACKGROUND_COLOR, oldText, MENU_SECTION_OLD_SIZE);
  }

  void Slider::reset() {
    if(_setting->getValue() != _setting->getDefaultValue())
      setValue(_setting->getDefaultValue());
  }
