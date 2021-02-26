/*
  Input.h - Input-Library for Airduino.
*/

#ifndef Input_h
#define Input_h

#include "Arduino.h"
#include "../../util/Util.h"
#include "../slider/Slider.h"
#include "../button/Button.h"
#include "../multiselect/Multiselect.h"
#include "../number/Number.h"
#include "../select/Select.h"

class Input {
  public:
    Input(void);
    Input(Setting* setting);
    void setSection(byte section);
    void setup();
    void reset(boolean active);
    void handleTouch(TSPoint p);

  private:
    void draw();
    void clear();

    byte _inputSection;
    Setting* _setting;
    Slider _slider;
    Button _button;
    Multiselect _multiselect;
    Number _number;
    Select _select;
};

#endif
