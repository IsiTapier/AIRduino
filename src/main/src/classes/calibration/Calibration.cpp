/*
  Calibration.cpp - Calibration-Library for Librarys.
*/

// #include "Arduino.h"
#include "Calibration.h"

Calibration::Calibration(int lowestSensor, int highestSensor, int lowestPPM, int highestPPM) {
  _lowestSensor = lowestSensor;
  _highestSensor = highestSensor;
  _lowestPPM = lowestPPM;
  _highestPPM = highestPPM;
}

int Calibration::getLowestSensor() {
  return(_lowestSensor);
}

int Calibration::getHighestSensor() {
  return(_highestSensor);
}

int Calibration::getLowestPPM() {
  return(_lowestPPM);
}

int Calibration::getHighestPPM() {
  return(_highestPPM);
}
