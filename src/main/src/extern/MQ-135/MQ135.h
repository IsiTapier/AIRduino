
/**************************************************************************/
/*!
@file     MQ135.h
@author   G.Krocker (Mad Frog Labs)
@license  GNU GPLv3

First version of an Arduino Library for the MQ135 gas sensor
TODO: Review the correction factor calculation. This currently relies on
the datasheet but the information there seems to be wrong.

@section  HISTORY

v1.0 - First release
*/
/**************************************************************************/
#ifndef MQ135_H
#define MQ135_H

#include "Arduino.h"
#include "../../util/Util.h"

//analog read resolution
#define RESOLUTION 12
//analog read max voltage
#define VOLTAGE 5
/// The load resistance on the board
#define RLOAD 20
  /// Calibration resistance at atmospheric CO2 level 222.15	1038.00



#define RZERO 542.77
#define RZEROB 1690.58	
#define RZEROC 2838.39
#define RZEROD 533.33
#define RZEROE 1661.18
#define RZEROF 2789.02
/// Parameters for calculating ppm of CO2 from sensor resistance
#define PARA 116.6020682
#define PARB 2.769034857

/// Parameters to model temperature and humidity dependence
#define CORA 0.00035
#define CORB 0.02718
#define CORC 1.39538
#define CORD 0.0018

/// Atmospheric CO2 level for calibration purposes
#define ATMOCO2 397.13

class MQ135 {
 private:
  uint8_t _pin;

  float getAnalogValue();
  float getAnalogValue(int test);
  float getCorrectionFactor(float t, float h);
  float getResistance(float analog);
  float getResistance(float analog, int test);
  float getResistance(float analog, String test);
  float getResistance(float analog, float t, float h);
  float getResistance(float analog, float t, float h, int test);
  float getResistance(float analog, float t, float h, String test);

 public:
  MQ135(uint8_t pin);
  float getPPM(float analog);
  float getPPM(float analog, int text);
  float getPPM(float analog, String text);
  float getPPM(float analog, float t, float h);
  float getPPM(float analog, float t, float h, int test);
  float getPPM(float analog, float t, float h, String test);
  float getRZero(float analog);
  float getRZero(float analog, int text);
  float getRZero(float analog, String text);
  float getRZero(float analog, float t, float h);
  float getRZero(float analog, float t, float h, int test);
  float getRZero(float analog, float t, float h, String test);

};
#endif
