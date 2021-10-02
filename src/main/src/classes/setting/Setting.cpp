/*
 Setting.cpp - Setting-Library for Librarys.
*/

#include "Setting.h"
#include "../../util/Util.h"
#include "../../display/Display.h"

Setting::Setting(void) {}
Setting::Setting(SettingType type, const char* key, std::vector<String> title, short defaultValue, const char* debugMessage, short minValue, short maxValue, std::vector<std::vector<String>> names,  boolean colored) { //TODO DATATYPE
  _type = type;
  _key = key;
  _title = title;
  _value = defaultValue;
  _oldValue = defaultValue;
  _defaultValue = defaultValue;
  _minValue = minValue;
  _maxValue = maxValue;
  _colored = colored;
  _names = names;
  _debugMessage = debugMessage;
}

short Setting::getValue() {
  return _value;
}

short* Setting::getValuePointer() {
  return &_value;
}

short Setting::getOldValue() {
  return _oldValue;
}

boolean Setting::hasChanged() {
  return _value == _oldValue ? false : true;
}

boolean Setting::equals(short value) {
  return _value == value ? true : false;
}

boolean Setting::equaled(short value) {
  return _oldValue == value ? true : false;
}

short Setting::getDefaultValue() {
  return _defaultValue;
}

short Setting::getMinValue() {
  return _minValue;
}

short Setting::getMaxValue() {
  return _maxValue;
}

void Setting::setValue(short value, boolean upload) {
  if(getTitle() == developperSettings.getTitle() && (graph_speed.getValue() != graph_speed.getMinValue() || ventilating_timeout.getValue() != ventilating_timeout.getMinValue()) && value != 0) return;
  if(value > _maxValue)
    value = _maxValue;
  if(value < _minValue)
    value = _minValue;
  
  _oldValue = _value;
  _value = value;
  
  debug(SPAMM, DATABASE, "Updated " + String(_key) + ", " + String(_value));
  if(strlen(_key) != 0  && upload) {
    config_update(_key, _value);
  }
}

void Setting::shiftValue(boolean up, boolean upload) {
  if(up && _value >= _maxValue)
    setValue(_minValue, upload);
  else if(!up && _value <= _minValue)
    setValue(_maxValue, upload);
  else if(up)
    setValue(_value+1);
  else
    setValue(_value-1);
  
}

SettingType Setting::getType() {
  return _type;
}

String Setting::getTitle() {
  return GETLANGUAGE(_title, false);
}

short Setting::getSize() {
  return _names.size();
}

String Setting::getName(boolean reverse) {
  if(reverse)
    return GETNAME(_type == SELECT ? !_value : _oldValue, _type == SELECT ? false : reverse);
  else
    return GETNAME(_value, reverse);
}

boolean Setting::getColored() {
  return _colored;
}

const char* Setting::getDebugMessage() {
  return _debugMessage;
}
