/*
  Util.cpp - Util-Library for Librarys.
*/

#include "Arduino.h"
#include "Util.h"

  //State
  extern State Util::getStateOf(int value) {
         if (value <= LIMIT_GOOD)
      return((State) GOOD);
    else if (value <= LIMIT_MEDIUM)
      return((State) MEDIUM);
    else if (value < LIMIT_BLINK)
      return((State) BAD);
    else if (value >= LIMIT_BLINK)
      return((State) BLINK);
    else if (value >= LIMIT_PIEP)
      return((State) PIEP);
    else
      return((State) BAD);
  }

  extern Calibration Util::calibration[] = {
      Calibration(320, 600, 400, 1000),
      Calibration(55, 65, 400, 1000),
      Calibration(40, 60, 400, 1000)
  };



  extern void Util::setup() {
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
  }

  extern void Util::loop() {

  }

  extern long Util::map(double x, long in_min, long in_max, long out_min, long out_max) {
    return round((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
  }

  extern int Util::map(int value, int startA, int startB, float factor) {
    value = value - startA;
    value = value * factor;
    value = value + startB;
    return(value);
  }

  extern void Util::rgb(int red, int green, int blue) {
    /*analogWrite(LED_RED, red);
    analogWrite(LED_GREEN, green);
    analogWrite(LED_BLUE, blue);*/
  }

  extern int Util::average(int averageArray[], int averageStart, int averageEnd) {
    int sum = 0;
    for (int i = averageStart; i < averageEnd; i++) {
      sum = sum + averageArray[i];
    }
    return (sum / (averageEnd - averageStart));
  }

  extern void Util::debug(String title, float value) {
    Serial.print(title + ": ");
    if(floor(value) == ceil(value))
      Serial.println((int) value);
    else
      Serial.println(value);

  }
