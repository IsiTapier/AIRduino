/*
  Setting.h - Setting-Library for Airduino.
*/

#ifndef Setting_h
#define Setting_h

#include "Arduino.h"

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
    Setting(SettingType type, char* title = "", short defaultValue = 0, boolean colored = false, char* debugMessage = "", std::vector<String> names = {"", ""});  //TODO DATATYPE
    short getValue();
    short getDefaultValue();
    void setValue(short value);
    SettingType getType();
    char* getTitle();
    short getSize();
    boolean getColored();
    String getName(boolean reverse = false);
    char* getDebugMessage();

  private:
    SettingType _type;
    char* _title;
    short _value;
    short _defaultValue;
    char* _debugMessage;
    char* _nameOn;
    char* _nameOff;
    boolean _colored;
    std::vector<String> _names;
};

#endif
