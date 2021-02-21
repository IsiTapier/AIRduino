/*
  Setting.h - Setting-Library for Airduino.
*/

#ifndef Setting_h
#define Setting_h

#include "Arduino.h"

#include "WiFi.h"
#include "WiFiClient.h"
#include "WiFiServer.h"
#include "../../extern/PubSubClient/src/PubSubClient.h"
#include "../../extern/ArduinoUniqueID/src/ArduinoUniqueID.h"

extern WiFiClient espClient;
extern PubSubClient client;

extern int device_id;

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
    Setting(SettingType type, String key, char* title = "", short defaultValue = 0, char* debugMessage = "", short minValue = 0, short maxValue = 1, std::vector<String> names = {"", ""}, boolean colored = true);
    short getValue();
    short getOldValue();
    short getDefaultValue();
    short getMinValue();
    short getMaxValue();
    void setValue(short value, boolean upload = true);
    SettingType getType();
    char* getTitle();
    short getSize();
    boolean getColored();
    String getName(boolean reverse = false);
    char* getDebugMessage();

  private:
    SettingType _type;
    String _key;
    char* _title;
    short _value;
    short _oldValue;
    short _defaultValue;
    short _minValue;
    short _maxValue;
    char* _debugMessage;
    char* _nameOn;
    char* _nameOff;
    boolean _colored;
    std::vector<String> _names;
};

#endif
