/**************************************************************************/
/*!
@file     MQ135.cpp
@author   G.Krocker (Mad Frog Labs)
@license  GNU GPLv3

First version of an Arduino Library for the MQ135 gas sensor
TODO: Review the correction factor calculation. This currently relies on
the datasheet but the information there seems to be wrong.

@section  HISTORY

v1.0 - First release
*/
/**************************************************************************/

#include "MQ135.h"

/**************************************************************************/
/*!
@brief  Default constructor

@param[in] pin  The analog input pin for the readout of the sensor
*/
/**************************************************************************/

MQ135::MQ135(uint8_t pin) {
  _pin = pin;
}


/**************************************************************************/
/*!
@brief  Get the analog value to calculate resistance

@return The calculated correction factor
*/
/**************************************************************************/
float MQ135::getAnalogValue() {
  unsigned long analogValue = 0;
  for (long i = 0; i < AVERAGING_MEASUREMENTS; i++) {
    // analogValue += analogRead(GAS_SENSOR);
    delay(STAGE_TIME / AVERAGING_MEASUREMENTS);
  }
  float back = analogValue / AVERAGING_MEASUREMENTS;
  if (general::debugSensor.getValue() && general::debugPriority.getValue())
    debug(SPAMM, SENSOR, "analog value", back);
  return back;
}

float MQ135::getAnalogValue(int test) {
  unsigned long analogValue = 0;
  for (long i = 0; i < AVERAGING_MEASUREMENTS; i++) {
    // analogValue += analogRead(GAS_SENSOR);
    delay(STAGE_TIME / AVERAGING_MEASUREMENTS);
  }
  float back = analogValue / AVERAGING_MEASUREMENTS;
  if (general::debugSensor.getValue() && general::debugPriority.getValue())
    debug(SPAMM, SENSOR, "analog value", back);
  Serial.print(String(back)+"\t");
  return back;
}

/**************************************************************************/
/*!
@brief  Get the correction factor to correct for temperature and humidity

@param[in] t  The ambient air temperature
@param[in] h  The relative humidity

@return The calculated correction factor
*/
/**************************************************************************/
float MQ135::getCorrectionFactor(float t, float h) {
  float back = CORA * t * t - CORB * t + CORC - (h-33.)*CORD;
  if (general::debugSensor.getValue() && general::debugPriority.getValue())
    debug(SPAMM, SENSOR, "correction factor", back);
  return back;
}

/**************************************************************************/
/*!
@brief  Get the resistance of the sensor, ie. the measurement value

@return The sensor resistance in kOhm
*/
/**************************************************************************/
float MQ135::getResistance(float analog) {
  float back = (((pow(2, RESOLUTION)-1)/analog)*5 - 1.)*RLOAD;
  if (general::debugSensor.getValue() && general::debugPriority.getValue())
    debug(SPAMM, SENSOR, "resistance", back);
  return back;
}

float MQ135::getResistance(float analog, int test) {
  float back = (((pow(2, RESOLUTION)-1)/analog)*5 - 1.)*RLOADB;
  if (general::debugSensor.getValue() && general::debugPriority.getValue())
    debug(SPAMM, SENSOR, "resistance", back);
  return back;
}

float MQ135::getResistance(float analog, String test) {
  float back = (((pow(2, RESOLUTION)-1)/analog)*5 - 1.)*RLOADC;
  if (general::debugSensor.getValue() && general::debugPriority.getValue())
    debug(SPAMM, SENSOR, "resistance", back);
  return back;
}

/**************************************************************************/
/*!
@brief  Get the resistance of the sensor, ie. the measurement value corrected
        for temp/hum

@param[in] t  The ambient air temperature
@param[in] h  The relative humidity

@return The corrected sensor resistance kOhm
*/
/**************************************************************************/
float MQ135::getResistance(float analog, float t, float h) {
  float back = getResistance(analog)/getCorrectionFactor(t, h);
  if (general::debugSensor.getValue() && general::debugPriority.getValue())
    debug(SPAMM, SENSOR, "corrected resistance", back);
  return back;
}

float MQ135::getResistance(float analog, float t, float h, int test) {
  float back = getResistance(analog, test)/getCorrectionFactor(t, h);
  if (general::debugSensor.getValue() && general::debugPriority.getValue())
    debug(SPAMM, SENSOR, "corrected resistance", back);
  return back;
}

float MQ135::getResistance(float analog, float t, float h, String test) {
  float back = getResistance(analog, test)/getCorrectionFactor(t, h);
  if (general::debugSensor.getValue() && general::debugPriority.getValue())
    debug(SPAMM, SENSOR, "corrected resistance", back);
  return back;
}

/**************************************************************************/
/*!
@brief  Get the ppm of CO2 sensed (assuming only CO2 in the air)

@return The ppm of CO2 in the air
*/
/**************************************************************************/
float MQ135::getPPM(float analog) {
  float back = PARA * pow((getResistance(analog)/RZERO), -PARB);
  if (general::debugSensor.getValue() && general::debugPriority.getValue())
    debug(SPAMM, SENSOR, "ppm", back);
  return back;
}

float MQ135::getPPM(float analog, int test) {
  float back = PARA * pow((getResistance(analog, test)/RZEROB), -PARB);
  if (general::debugSensor.getValue() && general::debugPriority.getValue())
    debug(SPAMM, SENSOR, "ppm", back);
  return back;
}

float MQ135::getPPM(float analog, String test) {
  float back = PARA * pow((getResistance(analog, test)/RZEROC), -PARB);
  if (general::debugSensor.getValue() && general::debugPriority.getValue())
    debug(SPAMM, SENSOR, "ppm", back);
  return back;
}

/**************************************************************************/
/*!
@brief  Get the ppm of CO2 sensed (assuming only CO2 in the air), corrected
        for temp/hum

@param[in] t  The ambient air temperature
@param[in] h  The relative humidity

@return The ppm of CO2 in the air
*/
/**************************************************************************/
float MQ135::getPPM(float analog, float t, float h) {
  float back = PARA * pow((getResistance(analog, t, h)/RZEROD), -PARB);
  if (general::debugSensor.getValue() && general::debugPriority.getValue())
    debug(SPAMM, SENSOR, "corrected ppm", back);
  return back;
}

float MQ135::getPPM(float analog, float t, float h, int test) {
  float back = PARA * pow((getResistance(analog, t, h, test)/RZEROE), -PARB);
  if (general::debugSensor.getValue() && general::debugPriority.getValue())
    debug(SPAMM, SENSOR, "corrected ppm", back);
  return back;
}

float MQ135::getPPM(float analog, float t, float h, String test) {
  float back = PARA * pow((getResistance(analog, t, h, test)/RZEROF), -PARB);
  if (general::debugSensor.getValue() && general::debugPriority.getValue())
    debug(SPAMM, SENSOR, "corrected ppm", back);
  return back;
}

/**************************************************************************/
/*!
@brief  Get the resistance RZero of the sensor for calibration purposes

@return The sensor resistance RZero in kOhm
*/
/**************************************************************************/
float MQ135::getRZero(float analog) {
  float back = getResistance(analog) * pow((ATMOCO2/PARA), (1./PARB));
  if (general::debugSensor.getValue() && general::debugPriority.getValue())
    debug(SPAMM, SENSOR, "clean air resistance", back);
  return back;
}

float MQ135::getRZero(float analog, int test) {
  float back = getResistance(analog, test) * pow((ATMOCO2/PARA), (1./PARB));
  if (general::debugSensor.getValue() && general::debugPriority.getValue())
    debug(SPAMM, SENSOR, "clean air resistance", back);
  return back;
}

float MQ135::getRZero(float analog, String test) {
  float back = getResistance(analog, test) * pow((ATMOCO2/PARA), (1./PARB));
  if (general::debugSensor.getValue() && general::debugPriority.getValue())
    debug(SPAMM, SENSOR, "clean air resistance", back);
  return back;
}

/**************************************************************************/
/*!
@brief  Get the corrected resistance RZero of the sensor for calibration
        purposes

@param[in] t  The ambient air temperature
@param[in] h  The relative humidity

@return The corrected sensor resistance RZero in kOhm
*/
/**************************************************************************/
float MQ135::getRZero(float analog, float t, float h) {
  float back = getResistance(analog, t, h) * pow((ATMOCO2/PARA), (1./PARB));
  if (general::debugSensor.getValue() && general::debugPriority.getValue())
    debug(SPAMM, SENSOR, "corrected clean air resistance", back);
  return back;
}

float MQ135::getRZero(float analog, float t, float h, int test) {
  float back = getResistance(analog, t, h, test) * pow((ATMOCO2/PARA), (1./PARB));
  if (general::debugSensor.getValue() && general::debugPriority.getValue())
    debug(SPAMM, SENSOR, "corrected clean air resistance", back);
  return back;
}

float MQ135::getRZero(float analog, float t, float h, String test) {
  float back = getResistance(analog, t, h, test) * pow((ATMOCO2/PARA), (1./PARB));
  if (general::debugSensor.getValue() && general::debugPriority.getValue())
    debug(SPAMM, SENSOR, "corrected clean air resistance", back);
  return back;
}
