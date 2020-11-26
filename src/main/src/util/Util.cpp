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


  static void Util::setup() {
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
  }

  static void Util::loop() {

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

  static void Util::debug(String title, int value) {
    Serial.print(title + ": ");
    Serial.println(value);
  }
