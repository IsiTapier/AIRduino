/*
  Slider.h - Slider-Library for Airduino.
*/

#ifndef Slider_h
#define Slider_h

#include "Arduino.h"
#include "../../util/Util.h"

class Slider {
  public:
    Slider(void);
    Slider(String textOff, String testOn, boolean defaultValue = false, boolean colored = true, String debugMessage = "");
    Slider& setSection(int section);
    int getSection();
    void init();
    void reset();
    void checkTouch(TSPoint p);

  private:
    boolean getValue();
    void setValue(boolean value);
    void changeValue();
    void draw();

    String _textOff;
    String _textOn;
    boolean _value;
    boolean _defaultValue;
    boolean _colored;
    int _section;
    String _debugMessage;

    String text;
    String oldText;
};

#endif
