/*
 Slider.cpp - Slider-Library for Librarys.
*/

#include "Slider.h"

  extern int _section;
  extern boolean _value;
  extern boolean _colored;
  extern String _textOff;
  extern String _textOn;
  extern String _debugMessage;

  extern String text;
  extern String oldText;

  extern Slider::Slider(int section, String textOff, String textOn, boolean value, boolean colored, String debugMessage) {
    _section = section;
    _value = value;
    _colored = colored;
    _textOff = textOff;
    _textOn = textOn;
    _debugMessage = debugMessage;
  }

  extern void Slider::init() {
    drawBorder(0, MENU_SECTION_Y(_section) - MENU_SECTION_BORDER_SHIFT, MENU_SECTION_LENGTH, MENU_SECTION_HEIGHT + MENU_SECTION_BORDER_SHIFT, MENU_SECTION_BORDER_THICKNESS, TEXT_COLOR);
    text = _value ? _textOn : _textOff;
    dPrint(text, MENU_MARGIN_LEFT, MENU_SECTION_TEXT_Y(_section), MENU_SECTION_SIZE, TEXT_COLOR, 3);
    draw();
  }

  extern void Slider::checkTouch(TSPoint p) {
    if(p.isTouching(MENU_SLIDER_START_X, MENU_SLIDER_END_X, MENU_SLIDER_START_Y(_section), MENU_SLIDER_END_Y(_section))) {
      changeValue();
      if(_debugMessage != "")
        Serial.println(_debugMessage);
    }
  }

  extern boolean Slider::getValue() {
    return(_value);
  }

  extern void Slider::setValue(boolean value) {
    _value = value;
    if(mode == MENU)
      draw();
  }

  extern void Slider::changeValue() {
    setValue(!getValue());
  }

  extern void Slider::draw() {
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
