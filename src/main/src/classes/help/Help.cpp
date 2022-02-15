#include "Help.h"

#include "../../display/Display.h"
#include "../../meassure/Meassure.h"

Help::Help(String title, int x, int y, int size, int color, int datum, int timeDelay, int duration, int repeat) {
    _title = title;
    _x = x;
    _y = y;
    _size = size;
    _color = color;
    _datum = datum;
    _startMillis = 0;
    _endMillis = 0;
    _repeat = repeat;
    _timeDelay = timeDelay;
    _duration = duration;
    isDrawn = false;
}

void Help::loop() {
    if(!isDrawn) {
        if(millis() > _startMillis && millis() < _endMillis) {
            isDrawn = true;
            Serial.print("Draw "); Serial.println(_title);

            display.fillRoundRect(_x, _y, _title.length()*6*_size + 4, 8*_size, 3, _color);
            dPrint(_title, _x+2, _y, _size, BLACK, _datum);
        }
    } else {
        if(millis() > _endMillis) {
            isDrawn = false;
            Serial.print("Undraw "); Serial.println(_title);
            Display::enableInit();
            Display::loop();
            if(_repeat > 0) {
                _endMillis += _repeat;
                _startMillis += _repeat;
                Serial.println("Repeated Help");
                isDrawn = false;
            }
        }
    }
    if(_activated) return;
    _activated = true;
    _startMillis = millis() + _timeDelay;
    _endMillis = _startMillis + _duration;
}
