/*
  Calibration.cpp - Calibration-Library for Librarys.
*/

// #include "Arduino.h"
#include "Calibration.h"

int lowestSensor;
int highestSensor;
int lowestPPM;
int highestPPM;

Calibration::Calibration(int lowestSensor, int highestSensor, int lowestPPM, int highestPPM) {
  this -> lowestSensor = lowestSensor;
  this -> highestSensor = highestSensor;
  this -> lowestPPM = lowestPPM;
  this -> highestPPM = highestPPM;
}

int Calibration::getLowestSensor() {
  return(lowestSensor);
}

int Calibration::getHighestSensor() {
  return(highestSensor);
}

int Calibration::getLowestPPM() {
  return(lowestPPM);
}

int Calibration::getHighestPPM() {
  return(highestPPM);
}
