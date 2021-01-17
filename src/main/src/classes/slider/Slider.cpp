/*
 Slider.cpp - Slider-Library for Librarys.
*/

#include "Slider.h"

  Slider::Slider(void) {}

  Slider::Slider(String textOff, String textOn, boolean defaultValue, boolean colored, String debugMessage) {
    _value = defaultValue;
    _defaultValue = defaultValue;
    _colored = colored;
    _textOff = textOff;
    _textOn = textOn;
    _debugMessage = debugMessage;
  }

  Slider& Slider::setSection(int section) {
    _section = section;
    return(*this);
  }

  int Slider::getSection() {
    return(_section);
  }

  void Slider::init() {
    text = _value ? _textOn : _textOff;
    dPrint(text, MENU_MARGIN_LEFT, MENU_SECTION_TEXT_Y(_section), MENU_SECTION_SIZE, TEXT_COLOR, 3);
    draw();
  }

  void Slider::checkTouch(TSPoint p) {
    if(p.isTouching(MENU_SLIDER_START_X, MENU_SLIDER_END_X, MENU_SLIDER_START_Y(_section), MENU_SLIDER_END_Y(_section))) {
      changeValue();
      if(_debugMessage != "")
        Serial.println(_debugMessage);
    }
  }

  boolean Slider::getValue() {
    return(_value);
  }

  void Slider::setValue(boolean value) {
    _value = value;
    //Serial.println("Value changed to "+String(_value));
    if(mode == MENU)
      draw();
  }

  void Slider::changeValue() {
    /*setValue(!getValue());*/
    if(_value)
      setValue(false);
    else
      setValue(true);
  }

  void Slider::draw() {
    display.fillCircle(MENU_SLIDER_DOT_X(!_value), MENU_SLIDER_DOT_Y(_section), MENU_SLIDER_DOT_RADIUS, BACKGROUND_COLOR);
    display.fillRoundRect(MENU_SLIDER_START_X, MENU_SLIDER_START_Y(_section), MENU_SLIDER_LENGTH, MENU_SLIDER_HEIGHT, MENU_SLIDER_RADIUS, MENU_SLIDER_STRIP_COLOR(_value, _colored));
    display.drawRoundRect(MENU_SLIDER_START_X, MENU_SLIDER_START_Y(_section), MENU_SLIDER_LENGTH, MENU_SLIDER_HEIGHT, MENU_SLIDER_RADIUS, TEXT_COLOR);
    display.fillCircle(MENU_SLIDER_DOT_X(_value), MENU_SLIDER_DOT_Y(_section), MENU_SLIDER_DOT_RADIUS, MENU_SLIDER_DOT_COLOR(_value, _colored));
    text = _value ? _textOn : _textOff;
    oldText = _value ? _textOff : _textOn;
    dPrint(text, MENU_MARGIN_LEFT, MENU_SECTION_TEXT_Y(_section), MENU_SECTION_SIZE, TEXT_COLOR, 3, BACKGROUND_COLOR, oldText);
    if(_colored)
      display.drawCircle(MENU_SLIDER_DOT_X(_value), MENU_SLIDER_DOT_Y(_section), MENU_SLIDER_DOT_RADIUS, TEXT_COLOR);
  }

  void Slider::reset() {
    if(getValue() != _defaultValue)
      setValue(_defaultValue);
  }
