/*
 Input.cpp - Input-Library for Librarys.
*/

#include "Input.h"

  Input::Input(void) {}
  Input::Input(Setting setting) {
    _settings = setting;
    if(_settings.getType() == SWITCH) {
      _slider = Slider(_settings);
    }
  }

  void Input::setSection(byte section) {
    _inputSection = section;
    if(_settings.getType() == SWITCH) {
      _slider.setSection(section);
    }
  }

  void Input::setup() {
    if(mode == MENU) {
      if(lastMode == MENU)
        clear();
      else
        draw();
      if(_settings.getType() == SWITCH) {
        _slider.init();
      }
    }
  }

  void Input::draw() {
    if(mode == MENU && lastMode != MENU) {
      drawBorder(0, MENU_SECTION_Y(_inputSection), MENU_SECTION_LENGTH, MENU_SECTION_HEIGHT + MENU_SECTION_BORDER_SHIFT, MENU_SECTION_BORDER_THICKNESS, TEXT_COLOR);
      Serial.println("Input "+String(_inputSection)+" drawn. Inputtype: "+((_settings.getType()==SWITCH)?"slider":"empty"));
    }
  }

  void Input::clear() {
    display.fillRect(MENU_SECTION_BORDER_THICKNESS, (MENU_SECTION_Y(_inputSection) + MENU_SECTION_BORDER_THICKNESS), (MENU_SECTION_LENGTH - MENU_SECTION_BORDER_THICKNESS - MENU_SECTION_BORDER_SHIFT), (MENU_SECTION_HEIGHT - MENU_SECTION_BORDER_THICKNESS), BACKGROUND_COLOR);
  }

  void Input::handleTouch(TSPoint p) {
    if(mode == MENU) {
      if(_settings.getType() == SWITCH) {
        _slider.checkTouch(p);
      }
    }
  }

  void Input::reset() {
    if(mode == MENU) {
      if(_settings.getType() == SWITCH) {
        _slider.reset();
      }
    }
  }
