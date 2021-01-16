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
    Input(InputType type, String textOff = "", String testOn = "", boolean value = false, boolean colored = true, String debugMessage = "");
    void setup(int section);
    void init();
    void handleTouch(TSPoint p);

  private:
    int _section;
    InputType _type;
    Slider _slider;

    String _textOff;
    String _textOn;
    boolean _value;
    boolean _colored;
    String _debugMessage;

};

#endif
