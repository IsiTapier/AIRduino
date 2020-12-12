/*
  Util.cpp - Util-Library for Librarys.
*/

#include "Arduino.h"
#include "Util.h"

  //State
  static State Util::states[7] = {State::VENTILATING, State::GOOD, State::MEDIUM, State::BAD, State::BLINK, State::PIEP};

  static int Util::getColor(State state) {
    switch ((int)state) {
      case -1:  return (CYAN);
        break;
      case 0: return (PPM_COLOR_N);
        break;
      case 1: return (PPM_COLOR_R);
        break;
      case 2: return (PPM_COLOR_A);
        break;
      default: return (PPM_COLOR_A);
        break;
    }
  }

  static State Util::getStateOf(int value) {
    for (int i = 0; i < 8; i++) {
      if ((int) states[i] == value)
        return (states[i]);
    }
  }

  static Calibration Util::calibration[] = {
      Calibration(37, 44, 400, 1000),
      Calibration(85, 110, 400, 1000),
      Calibration(40, 60, 400, 1000)
  };



  static void Util::setup() {
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
  }

  static void Util::loop() {

  }

  static long Util::map(double x, long in_min, long in_max, long out_min, long out_max) {
    return round((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
  }

  static int Util::map(int value, int startA, int startB, float factor) {
    value = value - startA;
    value = value * factor;
    value = value + startB;
    return(value);
  }

  static void Util::rgb(int red, int green, int blue) {
    analogWrite(LED_RED, red);
    analogWrite(LED_GREEN, green);
    analogWrite(LED_BLUE, blue);
  }

  static int Util::average(int averageArray[], int averageStart, int averageEnd) {
    int sum = 0;
    for (int i = averageStart; i < averageEnd; i++) {
      sum = sum + averageArray[i];
    }
    return (sum / (averageEnd - averageStart));
  }

  static void Util::debug(String title, float value) {
    Serial.print(title + ": ");
    if(floor(value) == ceil(value))
      Serial.println((int) value);
    else
      Serial.println(value);

  }
