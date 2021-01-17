/*
  Input.h - Input-Library for Airduino.
*/

#ifndef Input_h
#define Input_h

#include "Arduino.h"
#include "../../util/Util.h"
#include "../slider/Slider.h"


class Input {
  public:
    Input(void);
    Input(InputType type, String textOff = "", String testOn = "", boolean defaultValue = false, boolean colored = true, String debugMessage = "");
    Input(Input input, int section);
    void setSection(int section);
    void setup();
    void reset();
    void handleTouch(TSPoint p);

  private:
    void draw();

    int _inputSection;
    InputType _type;
    Slider _slider;

};

#endif
