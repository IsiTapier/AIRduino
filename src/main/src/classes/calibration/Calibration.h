/*
  Calibration.h - Calibration-Library for Airduino.
*/

#ifndef Calibration_h
#define Calibration_h

#include "Arduino.h"

class Calibration {

  public:
    Calibration(int lowestSensor, int highestSensor, int lowestPPM, int highestPPM);

    int getLowestSensor();
    int getHighestSensor();
    int getLowestPPM();
    int getHighestPPM();

  private:
    int _lowestSensor;
    int _highestSensor;
    int _lowestPPM;
    int _highestPPM;

};

#endif
