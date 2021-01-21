/*
  Select.h - Select-Library for Airduino.
*/

#ifndef Select_h
#define Select_h

#include "Arduino.h"
#include "../../util/Util.h"
#include "../../util/settings/Settings.h"

class Select {
  public:
    Select(void);
    Select(Setting* setting);
    Select& setSection(byte section);
    byte getSection();
    void init();
    void reset();
    void checkTouch(TSPoint p);

  private:
    void setValue(boolean value);
    void draw();
    void write();

    Setting* _setting;
    byte _section;
};

#endif
