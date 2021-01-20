/*
  Slider.h - Slider-Library for Airduino.
*/

#ifndef Slider_h
#define Slider_h

#include "Arduino.h"
#include "../../util/Util.h"
#include "../../util/settings/Settings.h"

class Slider {
  public:
    Slider(void);
    Slider(Setting* setting);
    Slider& setSection(byte section);
    byte getSection();
    void init();
    void reset();
    void checkTouch(TSPoint p);

  private:
    void setValue(boolean value);
    void changeValue();
    void draw();
    void write(boolean init = false);

    Setting* _setting;
    byte _section;
};

#endif
