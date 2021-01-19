/*
  Setting.h - Setting-Library for Airduino.
*/

#ifndef Setting_h
#define Setting_h

#include "Arduino.h"
#include "../../util/Util.h"

enum SettingType {
  EMPTY,
  ACTIVATOR,
  SELECT,
  MUTLISELECT,
  SWITCH,
  COLOR
};

class Setting {
  public:
    Setting(void);
    Setting(SettingType type, char* title = "", short defaultValue = 0, boolean colored = false, char* debugMessage = "", char* nameOn = "", char* nameOff = "");  //TODO DATATYPE
    short getValue();
    boolean getValueb();
    short getDefaultValue();
    boolean getDefaultValueb();
    void setValue(short value);
    void setValue(boolean value);
    SettingType getType();
    char* getTitle();
    boolean getColored();
    char* getName(boolean reverse = false);
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
};

#endif
