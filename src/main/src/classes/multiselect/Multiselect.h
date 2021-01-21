/*
  Multiselect.h - Multiselect-Library for Airduino.
*/

#ifndef Multiselect_h
#define Multiselect_h

#include "Arduino.h"
#include "../../util/Util.h"
#include "../../util/settings/Settings.h"

class Multiselect {
  public:
    Multiselect(void);
    Multiselect(Setting* setting);
    Multiselect& setSection(byte section);
    byte getSection();
    void init();
    void reset();
    void checkTouch(TSPoint p);

  private:
    void setValue(int value);
    void changeValue();
    void write(boolean init = false);

    Setting* _setting;
    byte _section;
};

#endif
