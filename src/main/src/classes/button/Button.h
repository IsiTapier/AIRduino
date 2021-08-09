/*
  Button.h - Button-Library for Airduino.
*/

#ifndef Button_h
#define Button_h

#include "Arduino.h"
#include "../../util/Util.h"
#include "../../util/settings/Settings.h"


class Button {
  public:
    Button(void);
    Button(Setting* setting);
    Button& setSection(byte section);
    byte getSection();
    void init();
    void checkTouch(TSPoint p);

  private:
    void write();
    void execute();

    Setting* _setting;
    byte _section;
};

#endif
