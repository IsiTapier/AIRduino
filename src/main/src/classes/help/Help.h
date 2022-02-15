#ifndef Help_h
#define Help_h

#include "Arduino.h"
#include "../../util/Util.h"
#include "../../util/settings/Settings.h"

class Help {
private:
    unsigned long _startMillis;
    unsigned long _endMillis;
    String _title;
    int _x;
    int _y;
    int _size;
    int _datum;
    int _color;
    int _gui;
    bool isDrawn;
    int _repeat;
    int _timeDelay;
    int _duration;
    bool _activated = false;

public:
    Help(String title, int x, int y, int size, int color, int datum, int timeDelay, int duration, int repeat = 0);
    void loop();
};

#endif
