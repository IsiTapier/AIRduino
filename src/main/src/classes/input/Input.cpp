/*
 Input.cpp - Input-Library for Librarys.
*/

#include "Input.h"

  Input::Input(void) {}
  Input::Input(Setting* setting) {
    _setting = setting;
    if(_setting->getType() == SWITCH)
      _slider = Slider(_setting);
    else if(_setting->getType() == ACTIVATOR)
      _button = Button(_setting);
    else if(_setting->getType() == MULTISELECT)
      _multiselect = Multiselect(_setting);
  }

  void Input::setSection(byte section) {
    _inputSection = section;
    if(_setting->getType() == SWITCH)
      _slider.setSection(section);
    else if(_setting->getType() == ACTIVATOR)
      _button.setSection(section);
    else if(_setting->getType() == MULTISELECT)
      _multiselect.setSection(section);
  }

  void Input::setup() {
    if(mode == MENU) {
      if(lastMode == MENU)
        clear();
      else
        draw();
      if(_setting->getType() == SWITCH)
        _slider.init();
      else if(_setting->getType() == ACTIVATOR)
        _button.init();
      else if(_setting->getType() == MULTISELECT)
        _multiselect.init();
    }
  }

  void Input::draw() {
    if(mode == MENU && lastMode != MENU) {
      drawBorder(0, MENU_SECTION_Y(_inputSection), MENU_SECTION_LENGTH, MENU_SECTION_HEIGHT + MENU_SECTION_BORDER_SHIFT, MENU_SECTION_BORDER_THICKNESS, TEXT_COLOR);
      if(general::debugMenu.getValue() && general::debug.getValue())
        Serial.println("Input "+String(_inputSection)+" drawn. Inputtype: "+(String)_setting->getType()/*(_setting->getType()==SWITCH)?"slider":(_setting->getType())"empty")*/);
    }
  }

  void Input::clear() {
    display.fillRect(MENU_SECTION_BORDER_THICKNESS, (MENU_SECTION_Y(_inputSection) + MENU_SECTION_BORDER_THICKNESS), (MENU_SECTION_LENGTH - MENU_SECTION_BORDER_THICKNESS - MENU_SECTION_BORDER_SHIFT), (MENU_SECTION_HEIGHT - MENU_SECTION_BORDER_THICKNESS), BACKGROUND_COLOR);
  }

  void Input::handleTouch(TSPoint p) {
    if(mode == MENU) {
      if(_setting->getType() == SWITCH)
        _slider.checkTouch(p);
      else if(_setting->getType() == ACTIVATOR)
        _button.checkTouch(p);
      else if(_setting->getType() == MULTISELECT)
        _multiselect.checkTouch(p);
    }
  }

  void Input::reset() {
    if(mode == MENU) {
      if(_setting->getType() == SWITCH)
        _slider.reset();
      else if(_setting->getType() == MULTISELECT)
        _multiselect.reset();
    }
  }
