/*
  Number.h - Number-Library for Airduino.
*/

#ifndef Number_h
#define Number_h

#include "Arduino.h"
#include "../../util/Util.h"
#include "../../util/settings/Settings.h"

class Number {
  public:
    Number(void);
    Number(Setting* setting);
    Number& setSection(byte section);
    byte getSection();
    void init();
    void reset(boolean active);
    void checkTouch(TSPoint p);

  private:
    void setValue(short value, boolean active = true);
    void draw();
    void write(boolean init = false);

    Setting* _setting;
    byte _section;
};

#endif
