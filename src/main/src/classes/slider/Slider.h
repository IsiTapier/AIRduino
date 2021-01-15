/*
  Slider.h - Slider-Library for Airduino.
*/

#ifndef Slider_h
#define Slider_h

#include "Arduino.h"
#include "../../util/Util.h"

class Slider {
  public:
    Slider(int section, String textOff, String testOn, boolean value = false, boolean colored = true, String debugMessage = "");
    void init();
    void checkTouch(TSPoint p);
    boolean getValue();
    void setValue(boolean value);
    void changeValue();
    void draw();

  private:
    String _textOff;
    String _textOn;
    boolean _value;
    boolean _colored;
    int _section;
    String _debugMessage;

    String text;
    String oldText;
};

#endif
