/*
 Setting.cpp - Setting-Library for Librarys.
*/

#include "Arduino.h"
#include "Setting.h"

Setting::Setting(void) {}
Setting::Setting(SettingType type, String title, short defaultValue, boolean colored,  String debugMessage, std::vector<String> names) { //TODO DATATYPE
  _type = type;
  _title = title;
  _value = defaultValue;
  _defaultValue = defaultValue;
  _colored = colored;
  for(int i = 0; i < names.size(); i++) {
    _names[i] = names.at(i);
  }
  _debugMessage = debugMessage;
}

short Setting::getValue() {
  if(_type != SWITCH) {
    return _value;
  }
}

boolean Setting::getValueb() {
  if(_type == SWITCH) {
    if(_value > 0)
      return true;
    else
      return false;
  }
}

short Setting::getDefaultValue() {
  if(_type != SWITCH) {
    return _defaultValue;
  }
}

boolean Setting::getDefaultValueb() {
  if(_type == SWITCH) {
    if(_defaultValue > 0)
      return true;
    else
      return false;
  }
}

void Setting::setValue(short value) {
  if(_type != SWITCH)
    _value = value;
}

void Setting::setValue(boolean value) {
  if(_type == SWITCH)
    _value = value;
}

SettingType Setting::getType() {
  return _type;
}

String Setting::getTitle() {
  return _title;
}

String Setting::getName(boolean reverse) {
  if(reverse)
    return _names[(int)!(boolean)_value];
  else
    return _names[_value];
}

boolean Setting::getColored() {
  return _colored;
}

String Setting::getDebugMessage() {
  return _debugMessage;
}
