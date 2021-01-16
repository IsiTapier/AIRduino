/*
 Input.cpp - Input-Library for Librarys.
*/

#include "Input.h"

  extern int _section;
  extern InputType _type;
  extern Slider _slider;

  extern String _textOff;
  extern String _textOn;
  extern boolean _value;
  extern boolean _colored;
  extern String _debugMessage;


  extern Input::Input(void) {}

  extern Input::Input(InputType type, String textOff, String textOn, boolean value, boolean colored, String debugMessage) {
    _type = type;

    _textOff = textOff;
    _textOn = textOn;
    _value = value;
    _colored = colored;
    _debugMessage = debugMessage;
  }

  extern void Input::setup(int section) {
    _section = section;
    if(_type == SLIDER) {
      _slider = Slider(_section, _textOff, _textOn, _value, _colored, _debugMessage);
    }
  }

  extern void Input::init() {
    if(mode == MENU) {
      drawBorder(0, MENU_SECTION_Y(_section) - MENU_SECTION_BORDER_SHIFT, MENU_SECTION_LENGTH, MENU_SECTION_HEIGHT + MENU_SECTION_BORDER_SHIFT, MENU_SECTION_BORDER_THICKNESS, TEXT_COLOR);
      if(_type == SLIDER) {
        _slider.init();
      }
    }
  }

  extern void Input::handleTouch(TSPoint p) {
    if(mode == MENU) {
      if(_type == SLIDER) {
        _slider.checkTouch(p);
      }
    }
  }
