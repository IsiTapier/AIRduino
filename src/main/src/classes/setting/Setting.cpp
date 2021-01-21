/*
 Setting.cpp - Setting-Library for Librarys.
*/

#include "Arduino.h"
#include "Setting.h"

Setting::Setting(void) {}
Setting::Setting(SettingType type, char* title, short defaultValue, boolean colored,  char* debugMessage, std::vector<String> names) { //TODO DATATYPE
  _type = type;
  _title = title;
  _value = defaultValue;
  _defaultValue = defaultValue;
  _colored = colored;
  _names = names;
  /*for(int i = 0; i < names.size(); i++) {
    _names[i] = names.at(i);
  }*/
  _debugMessage = debugMessage;
}

short Setting::getValue() {
  return _value;
}

short Setting::getDefaultValue() {
  return _defaultValue;
}

void Setting::setValue(short value) {
  _value = value;
}

SettingType Setting::getType() {
  return _type;
}

char* Setting::getTitle() {
  return _title;
}

short Setting::getSize() {
  return _names.size()-1;
}

String Setting::getName(boolean reverse) {
  if(reverse) {
    if(_type == SWITCH) {
      return _names.at(!_value);
    } else if(_value > 0) {
      return _names.at(_value-1);
    } else {
      return _names.back();
    }
  } else {
    return _names.at(_value);
  }
}

boolean Setting::getColored() {
  return _colored;
}

char* Setting::getDebugMessage() {
  return _debugMessage;
}
