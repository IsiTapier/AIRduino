/*
 Setting.cpp - Setting-Library for Librarys.
*/

#include "Setting.h"
#include "../../util/Util.h"

Setting::Setting(void) {}
Setting::Setting(SettingType type, String key, std::vector<String> title, short defaultValue, char* debugMessage, short minValue, short maxValue, std::vector<std::vector<String>> names,  boolean colored) { //TODO DATATYPE
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

short Setting::getOldValue() {
  return _oldValue;
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
  if(value > _maxValue)
    value = _maxValue;
  if(value < _minValue)
    value = _minValue;
  _oldValue = _value;
  _value = value;
  if(_key != "" && upload) {
    config_update(_key, _value);
  }
}

SettingType Setting::getType() {
  return _type;
}

String Setting::getTitle() {
  return GETLANGUAGE(_title);
}

short Setting::getSize() {
  return _names.size();
}

String Setting::getName(boolean reverse) {
  if(getSize() > _value) {
    if(reverse) {
      if(_type == SWITCH) {
        return GETLANGUAGE(_names.at(!_value));
      } else if(_value > 0) {
        return GETLANGUAGE(_names[_value-1]);
      } else {
        return GETLANGUAGE(_names[getSize()]);
      }
    } else {
      return GETLANGUAGE(_names[_value]);
    }
  } else {
    return GETLANGUAGE(_title);
  }
}

boolean Setting::getColored() {
  return _colored;
}

char* Setting::getDebugMessage() {
  return _debugMessage;
}
