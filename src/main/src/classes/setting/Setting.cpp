/*
 Setting.cpp - Setting-Library for Librarys.
*/

#include "Arduino.h"
#include "Setting.h"

Setting::Setting(void) {}
Setting::Setting(SettingType type, char* title, short defaultValue, boolean colored,  char* debugMessage, char* nameOn, char* nameOff) { //TODO DATATYPE
  _type = type;
  _title = title;
  _value = defaultValue;
  _defaultValue = defaultValue;
  _colored = colored;
  /*for(int i = 0; i < names.size(); i++) {
    _names[i] = names.at(i);
  }*/
  _nameOn = nameOn;
  _nameOff = nameOff;
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

char* Setting::getName(boolean reverse) {
  if(reverse) {
    if((boolean) _value)
      return(_nameOn);
    else
      return(_nameOff);
  } else {
    if((boolean) _value)
      return(_nameOff);
    else
      return(_nameOn);
  }
}

boolean Setting::getColored() {
  return _colored;
}

char* Setting::getDebugMessage() {
  return _debugMessage;
}
