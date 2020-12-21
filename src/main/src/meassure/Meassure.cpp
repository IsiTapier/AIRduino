/*
  Meassure.cpp - Meassure-Library for Librarys.
*/

#include "Arduino.h"
#include "Meassure.h"


  //   _____      _
  //  / ____|    | |
  // | (___   ___| |_ _   _ _ __
  //  \___ \ / _ \ __| | | | '_ \
  //  ____) |  __/ |_| |_| | |_) |
  // |_____/ \___|\__|\__,_| .__/
  //                       | |
  //                       |_|

  extern float Meassure::airCondition;
  extern float Meassure::airConditionRaw;
  extern float Meassure::airConditionLast;
  extern int Meassure::airConditionLowest;
  extern unsigned long Meassure::startTime;
  extern unsigned long Meassure::timer;
  extern float Meassure::gradient;
  extern int Meassure::value;
  extern int Meassure::values[AVERAGING_GRADIENT * 2];
  extern int Meassure::now;
  extern int Meassure::last;
  extern int Meassure::ppmSinceVentilation;
  extern State Meassure::state;
  extern int Meassure::colorState;
  extern int Meassure::led;
  extern int Meassure::lowest;
  extern int Meassure::red;
  extern int Meassure::green;
  extern int Meassure::blue;

  extern void Meassure::setup() {
    Serial.println("MEASSURE SETUP started");
    pinMode(PIEZO, OUTPUT);
    digitalWrite(PIEZO, LOW);
    Serial.println("Pins initialized");
    airConditionRaw = analogRead(GAS_SENSOR);
    airConditionLast = analogRead(GAS_SENSOR);
    startTime = millis();
    Serial.println("Variables initialized");
    Serial.println("MEASSURE SETUP complete");
    Serial.println();
  }

  extern void Meassure::loop() {
    meassureAirCondition();
    mapAirCondition();
    calculateGradient();
    checkVentilating();
    setState();
    writeLed();
    debug();
  }


  //Getter

  extern State Meassure::getState() {
    return(state);
  }

  extern int Meassure::getAirCondition() {
    return((int) airCondition);
  }

  extern int Meassure::getLowest() {
    return(lowest);
  }

  extern int Meassure::getPpmSinceVentilation() {
    return(ppmSinceVentilation);
  }

  extern unsigned long Meassure::getStartTime() {
    return(startTime);
  }

  extern void Meassure::debug() {
    if (DEBUG) {
      Serial.println("");
      Serial.println("Sensor");
      Serial.println("");
      Util::debug("Analog", analogRead(GAS_SENSOR));
      Util::debug("Average", airConditionRaw);
      Util::debug("Smoothed", airConditionLast);
      Util::debug("PPM", airCondition);
      Util::debug("gradient", gradient);
      Util::debug("airConditionLowest", airConditionLowest);
      Util::debug("Status", (int) state);
    }
  }

  //  __  __
  // |  \/  |
  // | \  / | ___  __ _ ___ ___ _   _ _ __ ___
  // | |\/| |/ _ \/ _` / __/ __| | | | '__/ _ \
  // | |  | |  __/ (_| \__ \__ \ |_| | | |  __/
  // |_|  |_|\___|\__,_|___/___/\__,_|_|  \___|

  extern void Meassure::meassureAirCondition() {
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

  extern void Meassure::mapAirCondition() {
    //to PPM
    if (airCondition <= Util::calibration[EEPROM.read(0)].getLowestSensor())
      airCondition = Util::calibration[EEPROM.read(0)].getLowestSensor();

    airCondition = Util::map(airCondition, Util::calibration[EEPROM.read(0)].getLowestSensor(), Util::calibration[EEPROM.read(0)].getHighestSensor(), Util::calibration[EEPROM.read(0)].getLowestPPM(), Util::calibration[EEPROM.read(0)].getHighestPPM());
  }


  extern void Meassure::calculateGradient() {
    // gradient ist die Differenz zwischen altem und neuem Wert
    //store last AirConditions
    for (int i = AVERAGING_GRADIENT * 2 - 1; i > 0; i--) {
      values[i] = values[i - 1];
    }
    values[0] = airCondition;

    //average AirConditions
    now = Util::average(values, 0, AVERAGING_GRADIENT);
    last = Util::average(values, AVERAGING_GRADIENT, AVERAGING_GRADIENT * 2);

    //gradient
    gradient = (float) now / last;
  }

  extern void Meassure::checkVentilating() {
    //start Ventilating
    if (gradient < MAX_DECREASE && state != VENTILATING) { // Wenn die Differenz die Hemmschwelle übersteigt: Wird Ventilating erkannt
      state = VENTILATING;
      ppmSinceVentilation = airCondition;
      Util::rgb(0, 0, 255);
      startTime = millis();
      digitalWrite(2, HIGH);
      delay(1000);
      digitalWrite(2, LOW);
    }

    //stop Ventilating
    if ((gradient > 1 && state == VENTILATING) || (millis() - timer >= VENTILATING_TIMEOUT && state == VENTILATING && timer != 0)) {
      // Wenn der Graph nach oben Steigt oder der Timer abgelaufen ist
      if (airCondition - airConditionLowest < MAX_INCREASE_LOWEST) {
        //Wenn
        airConditionLowest = ALPHA_LOWEST * airCondition + (1 - ALPHA_LOWEST) * airConditionLowest;
      }

      state = (State) 0;
      timer = 0;
      startTime = millis();


    } else if (gradient == 1 && timer == 0 && state == VENTILATING) {
      //Wenn der Graph nicht mehr steigt und der Timer noch nicht gestartet wurde
      //Ziel nach gewisser Zeit kein Abstieg -> Timer für Timeout wird gestartet
      timer = millis();
    }
    last = now;
  }

  extern void Meassure::setState() {

    // define colorState  -changed
    if(airCondition < LIMIT_GOOD) {
      colorState = Util::getStateOf(0);
    } else if (airCondition < LIMIT_MEDIUM) {
      colorState = (State) 1;
    } else if (airCondition < LIMIT_BAD) {
      colorState = (State) 2;
    }

    //colorState = map(airCondition, Util::calibration[EEPROM.read(0)].getLowestPPM(), Util::calibration[EEPROM.read(0)].getHighestPPM(), 0, 2);

    if (state == -1)
      return;
    state = Util::getStateOf(colorState);
    if (airCondition > LIMIT_PIEP)
      state = PIEP;
    else if (airCondition > LIMIT_ALARM)
      state = BLINK;

    if (state < -1)
      state = (State) -1;
    if (state > 4)
      state = (State) 4;
  }

  extern void Meassure::writeLed() {
    //map Values
    led = map(airCondition, lowest, LIMIT_ALARM, 0, 255);
    red = led;
    green = 255 - led;

    //turn on
    if (state != VENTILATING)
      Util::rgb(red, green, 0);
  }
