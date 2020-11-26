/*
  Calibration.h - Calibration-Library for Airduino.
*/

#ifndef Calibration_h
#define Calibration_h

#include "Arduino.h"

class Calibration {

  public:
    Calibration::Calibration(int lowestSensor, int highestSensor, int lowestPPM, int highestPPM);

    int Calibration::getLowestSensor();
    int Calibration::getHighestSensor();
    int Calibration::getLowestPPM();
    int Calibration::getHighestPPM();

  private:
    int lowestSensor;
    int highestSensor;
    int lowestPPM;
    int highestPPM;

};

#endif
