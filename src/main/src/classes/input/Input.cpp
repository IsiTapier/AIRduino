/*
 Input.cpp - Input-Library for Librarys.
*/

#include "Input.h"

  Input::Input(void) {}

  Input::Input(InputType type, String textOff, String textOn, boolean defaultValue, boolean colored, String debugMessage) {
    _type = type;
    if(_type == SLIDER) {
      _slider = Slider(textOff, textOn, defaultValue, colored, debugMessage);
    }
  }

  void Input::setSection(int section) {
    _inputSection = section;
    if(_type == SLIDER) {
      _slider.setSection(section);
    }
  }

  void Input::setup() {
    if(mode == MENU) {
      draw();
      if(_type == SLIDER) {
        _slider.init();
      }
    }
  }

  void Input::draw() {
    drawBorder(0, MENU_SECTION_Y(_inputSection) - MENU_SECTION_BORDER_SHIFT, MENU_SECTION_LENGTH, MENU_SECTION_HEIGHT + MENU_SECTION_BORDER_SHIFT, MENU_SECTION_BORDER_THICKNESS, TEXT_COLOR);
    Serial.println("Input "+String(_inputSection)+" drawn. Inputtype: "+((_type==SLIDER)?"slider":"empty"));
  }

  void Input::handleTouch(TSPoint p) {
    if(mode == MENU) {
      if(_type == SLIDER) {
        _slider.checkTouch(p);
      }
    }
  }

  void Input::reset() {
    if(mode == MENU) {
      if(_type == SLIDER) {
        _slider.reset();
      }
    }
  }
