/*
  Input.h - Input-Library for Airduino.
*/

#ifndef Input_h
#define Input_h

#include "Arduino.h"
#include "../../util/Util.h"
#include "../slider/Slider.h"
#include "../button/Button.h"

class Input {
  public:
    Input(void);
    Input(Setting* setting);
    void setSection(byte section);
    void setup();
    void reset();
    void handleTouch(TSPoint p);

  private:
    void draw();
    void clear();

    byte _inputSection;
    Setting* _setting;
    Slider _slider;
    Button _button;

};

#endif
