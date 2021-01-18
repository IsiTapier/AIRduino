/*
  Setting.h - Setting-Library for Airduino.
*/

#ifndef Setting_h
#define Setting_h

#include "Arduino.h"
#include "../../util/Util.h"

enum SettingType {
  EMPTY,
  AKTIVATOR,
  SELECT,
  SWITCH,
  COLOR
};

class Setting {
  public:
    Setting(void);
    Setting(SettingType type, String title = "", short defaultValue = 0, boolean colored = false, String debugMessage = "", std::vector<String> names = {});  //TODO DATATYPE
    short getValue();
    boolean getValueb();
    short getDefaultValue();
    boolean getDefaultValueb();
    void setValue(short value);
    void setValue(boolean value);
    SettingType getType();
    String getTitle();
    boolean getColored();
    String getName(boolean reverse = false);
    String getDebugMessage();

  private:
    SettingType _type;
    String _title;
    short _value;
    short _defaultValue;
    String _debugMessage;
    String _names[2];
    boolean _colored;
};

#endif
