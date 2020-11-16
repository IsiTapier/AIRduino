/*
  Meassure.cpp - Meassure-Library for Librarys.
*/

#include "Arduino.h"
#include "Meassure.h"

public:

  //   _____      _
  //  / ____|    | |
  // | (___   ___| |_ _   _ _ __
  //  \___ \ / _ \ __| | | | '_ \
  //  ____) |  __/ |_| |_| | |_) |
  // |_____/ \___|\__|\__,_| .__/
  //                       | |
  //                       |_|

  void Meassure::setup() {
    pinMode(PIEZO, OUTPUT);
    digitalWrite(PIEZO, LOW);

    airConditionRaw = analogRead(GAS_SENSOR);
    airConditionLast = analogRead(GAS_SENSOR);
    startTime = millis();
  }

  void Meassure::loop() {

  }

  void Meassure::debug() {
    if (DEBUG) {
      Serial.println("");
      Serial.println("Sensor");
      Serial.println("");
      debug("Analog", analogRead(GAS_SENSOR));
      debug("Average", airConditionRaw);
      debug("Smoothed", airConditionLast);
      debug("PPM", airCondition);
      debug("gradient", gradient);
      debug("airConditionLowest", airConditionLowest);
      debug("Status", (int) state);
    }
  }


  //  __  __
  // |  \/  |
  // | \  / | ___  __ _ ___ ___ _   _ _ __ ___
  // | |\/| |/ _ \/ _` / __/ __| | | | '__/ _ \
  // | |  | |  __/ (_| \__ \__ \ |_| | | |  __/
  // |_|  |_|\___|\__,_|___/___/\__,_|_|  \___|

  void Meassure::meassureAirCondition() {
    //Messung
    airCondition = 0;
    for (int i = 0; i < AVERAGING_MEASUREMENTS; i++) {
      value = analogRead(GAS_SENSOR);

      //Fehlmessungen überschreiben
      if (airConditionRaw * MAX_INCREASE < value && i == 0)
        airCondition = airCondition + airConditionRaw;
      else if (airCondition / i * MAX_INCREASE < value && i != 0)
        airCondition = airCondition + airCondition / i;
      else
        airCondition = airCondition + value;

      delay(STAGE_TIME / AVERAGING_MEASUREMENTS);
    }

    airCondition = airCondition / AVERAGING_MEASUREMENTS;
    airConditionRaw = airCondition;

    //Wert smoothen;
    airCondition = ALPHA_MEASUREMENTS * airCondition + (1 - ALPHA_MEASUREMENTS) * airConditionLast;

    airConditionLast = airCondition;
  }


  //  _____        _
  // |  __ \      | |
  // | |  | | __ _| |_ __ _
  // | |  | |/ _` | __/ _` |
  // | |__| | (_| | || (_| |
  // |_____/ \__,_|\__\__,_|

  void Meassure::mapAirCondition() {
    //to PPM
    /*if (airCondition < OSV_SENSOR)
      airCondition = OSV_SENSOR;
      if (airCondition > MAX_DISPLAYED_SENSOR)
      airCondition = MAX_DISPLAYED_SENSOR;*/

    //airCondition = map(airCondition, OSV_SENSOR, MAX_DISPLAYED_PPM / FACTOR, OSV_PPM, MAX_DISPLAYED_PPM);
    airCondition = map(airCondition, OSV_SENSOR, OSV_PPM, FACTOR);
    /*airCondition = airCondition - OSV_SENSOR;
      airCondition = airCondition * FACTOR;
      airCondition = airCondition + OSV_PPM;*/
  }


  void Meassure::calculateGradient() {
    // gradient ist die Differenz zwischen altem und neuem Wert
    //store last AirConditions
    for (int i = AVERAGING_GRADIENT * 2 - 1; i > 0; i--) {
      values[i] = values[i - 1];
    }
    values[0] = airCondition;

    //average AirConditions
    now = average(values, 0, AVERAGING_GRADIENT);
    last = average(values, AVERAGING_GRADIENT, AVERAGING_GRADIENT * 2);

    //gradient
    gradient = now / last;
  }

  void Meassure::checkVentilating() {
    //start Ventilating
    if (gradient < MAX_DECREASE && state != VENTILATING) { // Wenn die Differenz die Hemmschwelle übersteigt: Wird Ventilating erkannt
      state = VENTILATING;
      ppmSinceVentilation = airCondition;
      rgb(0, 0, 255);
      startTime = millis();
    }

    //stop Ventilating
    if ((gradient > 1 && state == VENTILATING) || (millis() - timer >= VENTILATING_TIMEOUT && state == VENTILATING && timer != 0)) {
      // Wenn der Graph nach oben Steigt oder der Timer abgelaufen ist
      if (airCondition - airConditionLowest < MAX_INCREASE_airConditionLowest) {
        //Wenn
        airConditionLowest = ALPHA_airConditionLowest * airCondition + (1 - ALPHA_airConditionLowest) * airConditionLowest;
      }

      state = 0;
      timer = 0;
      startTime = millis();


    } else if (gradient == 1 && timer == 0 && state == VENTILATING) {
      //Wenn der Graph nicht mehr steigt und der Timer noch nicht gestartet wurde
      //Ziel nach gewisser Zeit kein Abstieg -> Timer für Timeout wird gestartet
      timer = millis();
    }
    last = now;
  }

  void Meassure::setState() {
    colorState = map(airCondition, airConditionLowest, MAX_LIGHT, 0, 3);
    if (state == -1)
      return;
    state = getState(colorState);
    if (airCondition > MAX_PIEP)
      state = PIEP;
    else if (airCondition > MAX_BLINK)
      state = BLINK;

    if (state < -1)
      state = -1;
    if (state > 5)
      state = 5;
  }

private:
  int airCondition;
  int airConditionRaw;
  int airConditionLast;
  int airConditionairConditionLowest;
  unsigned long startTime;
  float gradient;
  State state;
  int value;
  int values[AVERAGING_GRADIENT * 2];
  int now;
  int last;
  int ppmSinceVentilation;
  int colorState;
