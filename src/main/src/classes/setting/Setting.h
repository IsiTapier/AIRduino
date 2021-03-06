/*
  Setting.h - Setting-Library for Airduino.
*/

#ifndef Setting_h
#define Setting_h

#include "Arduino.h"

#define GETLANGUAGE(vector, reverse) (vector.empty() ? "" : vector.at((vector.size() > (general::language.hasChanged() && reverse ? general::language.getOldValue() : general::language.getValue()) ? (general::language.hasChanged() && reverse ? general::language.getOldValue() : general::language.getValue())  : 0)))
#define GETNAME(value, reverse) GETLANGUAGE((_names.size() == 0 ? _title : _names.at(value - _minValue < _names.size() && value - _minValue >= 0  ? value - _minValue > _maxValue ? _maxValue : value - _minValue : 0)), reverse)

enum SettingType {
  EMPTY,
  ACTIVATOR,
  SELECT,
  MULTISELECT,
  SWITCH,
  COLOR,
  NUMBER
};

class Setting {
  public:
    Setting(void);
    Setting(SettingType type, const char* key = "", std::vector<String> title = {}, short defaultValue = 0, const char* debugMessage = "", short minValue = 0, short maxValue = 1, std::vector<std::vector<String>> names = {}, boolean colored = true);
    short getValue();
    short* getValuePointer();
    short getOldValue();
    boolean hasChanged();
    boolean equals(short value);
    boolean equaled(short value);
    short getDefaultValue();
    short getMinValue();
    short getMaxValue();
    void setValue(short value, boolean upload = true);
    void shiftValue(boolean up = true, boolean upload = true);
    SettingType getType();
    String getTitle();
    short getSize();
    boolean getColored();
    String getName(boolean reverse = false);
    const char* getDebugMessage();

  private:
    SettingType _type;
    const char* _key;
    std::vector<String> _title;
    short _value;
    short _oldValue;
    short _defaultValue;
    short _minValue;
    short _maxValue;
    const char* _debugMessage;
    char* _nameOn;
    char* _nameOff;
    boolean _colored;
    std::vector<std::vector<String>> _names;
};

#endif
