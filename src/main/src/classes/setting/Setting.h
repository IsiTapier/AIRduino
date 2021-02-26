/*
  Setting.h - Setting-Library for Airduino.
*/

#ifndef Setting_h
#define Setting_h

#include "Arduino.h"

#define GETLANGUAGE(vector) (vector.empty() ? "" : vector.at((vector.size() > general::language.getValue()) ? general::language.getValue() : 0))
#define GETNAME(value) GETLANGUAGE((_names.size() == 0 ? _title : _names.at(value - _minValue < _names.size() && value - _minValue >= 0  ? value - _minValue > _maxValue ? _maxValue : value - _minValue : 0)))

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
    Setting(SettingType type, String key = "", std::vector<String> title = {}, short defaultValue = 0, char* debugMessage = "", short minValue = 0, short maxValue = 1, std::vector<std::vector<String>> names = {}, boolean colored = true);
    short getValue();
    short getOldValue();
    boolean hasChanged();
    boolean equals(short value);
    boolean equaled(short value);
    short getDefaultValue();
    short getMinValue();
    short getMaxValue();
    void setValue(short value, boolean upload = true);
    SettingType getType();
    String getTitle();
    short getSize();
    boolean getColored();
    String getName(boolean reverse = false);
    char* getDebugMessage();

  private:
    SettingType _type;
    String _key;
    std::vector<String> _title;
    short _value;
    short _oldValue;
    short _defaultValue;
    short _minValue;
    short _maxValue;
    char* _debugMessage;
    char* _nameOn;
    char* _nameOff;
    boolean _colored;
    std::vector<std::vector<String>> _names;
};

#endif
