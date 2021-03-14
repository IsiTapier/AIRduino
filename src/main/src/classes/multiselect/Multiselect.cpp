/*
 Multiselect.cpp - Multiselect-Library for Librarys.
*/

#include "Multiselect.h"

using namespace general;

  Multiselect::Multiselect(void) {}

  Multiselect::Multiselect(Setting* setting) {
    _setting = setting;
  }

  Multiselect& Multiselect::setSection(byte section) {
    _section = section;
    return(*this);
  }

  byte Multiselect::getSection() {
    return(_section);
  }

  void Multiselect::init() {
    if(language.hasChanged())
      write(language.hasChanged()?false:true);
    write(true);
  }

  void Multiselect::checkTouch(TSPoint p) {
    if(p.isTouching(0, MENU_SECTION_LENGTH, MENU_SECTION_Y(_section), MENU_SECTION_END_Y(_section))) {
      changeValue();
      if(strlen(_setting->getDebugMessage()) != 0)
        debug(DEBUG, MENUD, _setting->getDebugMessage());
    }
  }

  void Multiselect::setValue(int value, boolean active) {
    _setting->setValue(value);
    debug(INFO, MENUD, "Value changed to", _setting->getValue());
    if(mode.equals(MENU) && active)
      write();
  }

  void Multiselect::changeValue() {
    if(_setting->getValue() < _setting->getMaxValue())
      setValue(_setting->getValue()+1);
    else
      setValue(_setting->getMinValue());
  }

  void Multiselect::write(boolean init) {
    String text = "";
    String oldText = "";
    text = _setting->getName();
    if(!init)
      oldText = _setting->getName(true);
    dPrint(text, MENU_SECTION_LENGTH/2, MENU_SECTION_TEXT_Y(_section), MENU_SECTION_SIZE, TEXT_COLOR, 4, BACKGROUND_COLOR, oldText, MENU_SECTION_OLD_SIZE);
  }

  void Multiselect::reset(boolean active) {
    if(_setting->getValue() != _setting->getDefaultValue())
      setValue(_setting->getDefaultValue(), active);
  }
