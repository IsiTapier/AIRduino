/*
 Slider.cpp - Slider-Library for Librarys.
*/

#include "Slider.h"

  Slider::Slider(void) {}

  Slider::Slider(Setting setting) {
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
    text = _setting.getName();
    dPrint(text, MENU_MARGIN_LEFT, MENU_SECTION_TEXT_Y(_section), MENU_SECTION_SIZE, TEXT_COLOR, 3);
    draw();
  }

  void Slider::checkTouch(TSPoint p) {
    if(p.isTouching(MENU_SLIDER_START_X, MENU_SLIDER_END_X, MENU_SLIDER_START_Y(_section), MENU_SLIDER_END_Y(_section))) {
      changeValue();
      if(_setting.getDebugMessage() != "")
        Serial.println(_setting.getDebugMessage());
    }
  }

  void Slider::setValue(boolean value) {
    _setting.setValue(value);
    //Serial.println("Value changed to "+String(_setting.getValue()));
    if(mode == MENU)
      draw();
  }

  void Slider::changeValue() {
    setValue(!_setting.getValueb());
  }

  void Slider::draw() {
    text = _setting.getName();
    oldText = _setting.getName(true);
    display.fillCircle(MENU_SLIDER_DOT_X(!_setting.getValueb()), MENU_SLIDER_DOT_Y(_section), MENU_SLIDER_DOT_RADIUS, BACKGROUND_COLOR);
    display.fillRoundRect(MENU_SLIDER_START_X, MENU_SLIDER_START_Y(_section), MENU_SLIDER_LENGTH, MENU_SLIDER_HEIGHT, MENU_SLIDER_RADIUS, MENU_SLIDER_STRIP_COLOR(_setting.getValueb(), _setting.getColored()));
    display.drawRoundRect(MENU_SLIDER_START_X, MENU_SLIDER_START_Y(_section), MENU_SLIDER_LENGTH, MENU_SLIDER_HEIGHT, MENU_SLIDER_RADIUS, TEXT_COLOR);
    display.fillCircle(MENU_SLIDER_DOT_X(_setting.getValueb()), MENU_SLIDER_DOT_Y(_section), MENU_SLIDER_DOT_RADIUS, MENU_SLIDER_DOT_COLOR(_setting.getValueb(), _setting.getColored()));
    dPrint(text, MENU_MARGIN_LEFT, MENU_SECTION_TEXT_Y(_section), MENU_SECTION_SIZE, TEXT_COLOR, 3, BACKGROUND_COLOR, oldText, MENU_SECTION_OLD_SIZE);
    if(_setting.getColored())
      display.drawCircle(MENU_SLIDER_DOT_X(_setting.getValueb()), MENU_SLIDER_DOT_Y(_section), MENU_SLIDER_DOT_RADIUS, TEXT_COLOR);
  }

  void Slider::reset() {
    if(_setting.getValueb() != _setting.getDefaultValueb())
      setValue(_setting.getDefaultValueb());
  }
