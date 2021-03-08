/*
 Input.cpp - Input-Library for Librarys.
*/

#include "Input.h"

using namespace general;

  Input::Input(void) {}
  Input::Input(Setting* setting) {
    _setting = setting;
    if(_setting->getType() == SWITCH)
      _slider = Slider(_setting);
    else if(_setting->getType() == ACTIVATOR)
      _button = Button(_setting);
    else if(_setting->getType() == MULTISELECT)
      _multiselect = Multiselect(_setting);
    else if(_setting->getType() == NUMBER)
      _number = Number(_setting);
    else if(_setting->getType() == SELECT)
      _select = Select(_setting);
  }

  void Input::setSection(byte section) {
    _inputSection = section;
    if(_setting->getType() == SWITCH)
      _slider.setSection(section);
    else if(_setting->getType() == ACTIVATOR)
      _button.setSection(section);
    else if(_setting->getType() == MULTISELECT)
      _multiselect.setSection(section);
    else if(_setting->getType() == NUMBER)
      _number.setSection(section);
    else if(_setting->getType() == SELECT)
      _select.setSection(section);
  }

  void Input::setup(boolean redraw) {
    if(!mode.hasChanged() && !theme.hasChanged() && !redraw)
      clear();
    else
      draw();
    if(_setting->getType() == SWITCH)
      _slider.init();
    else if(_setting->getType() == ACTIVATOR)
      _button.init();
    else if(_setting->getType() == MULTISELECT)
      _multiselect.init();
    else if(_setting->getType() == NUMBER)
      _number.init();
    else if(_setting->getType() == SELECT)
      _select.init();
  }

  void Input::draw() {
    if(_inputSection != 0)
      display.fillRect(0, MENU_SECTION_Y(_inputSection), MENU_SECTION_LENGTH+1, MENU_SECTION_BORDER_THICKNESS, TEXT_COLOR);
    debug(INFO, MENUD, "Input:", _inputSection, "drawn");
  }

  void Input::clear() {
    display.fillRect(0, (MENU_SECTION_Y(_inputSection) + MENU_SECTION_BORDER_THICKNESS), (MENU_SECTION_LENGTH+1), (MENU_SECTION_HEIGHT - MENU_SECTION_BORDER_THICKNESS), BACKGROUND_COLOR);
  }

  void Input::handleTouch(TSPoint p) {
    if(mode.equals(MENU)) {
      if(_setting->getType() == SWITCH)
        _slider.checkTouch(p);
      else if(_setting->getType() == ACTIVATOR)
        _button.checkTouch(p);
      else if(_setting->getType() == MULTISELECT)
        _multiselect.checkTouch(p);
      else if(_setting->getType() == NUMBER)
        _number.checkTouch(p);
      else if(_setting->getType() == SELECT)
        _select.checkTouch(p);
    }
  }

  void Input::reset(boolean active) {
    if(_setting->getType() == SWITCH)
      _slider.reset(active);
    else if(_setting->getType() == MULTISELECT)
      _multiselect.reset(active);
    else if(_setting->getType() == NUMBER)
      _number.reset(active);
    else if(_setting->getType() == SELECT)
      _select.reset();
  }
