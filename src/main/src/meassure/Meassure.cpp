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

  static int Meassure::airCondition;
  static int Meassure::airConditionRaw;
  static int Meassure::airConditionLast;
  static int Meassure::airConditionLowest;
  static unsigned long Meassure::startTime;
  static unsigned long Meassure::timer;
  static float Meassure::gradient;
  static int Meassure::value;
  static int Meassure::values[AVERAGING_GRADIENT * 2];
  static int Meassure::now;
  static int Meassure::last;
  static int Meassure::ppmSinceVentilation;
  static State Meassure::state;
  static int Meassure::colorState;
  static int Meassure::led;
  static int Meassure::lowest;
  static int Meassure::red;
  static int Meassure::green;
  static int Meassure::blue;

  static void Meassure::setup() {
    pinMode(PIEZO, OUTPUT);
    digitalWrite(PIEZO, LOW);

    airConditionRaw = analogRead(GAS_SENSOR);
    airConditionLast = analogRead(GAS_SENSOR);
    startTime = millis();
  }

  static void Meassure::loop() {
    meassureAirCondition();
    mapAirCondition();
    calculateGradient();
    checkVentilating();
    setState();
    writeLed();
    debug();
  }


  //Getter

  static State Meassure::getState() {
    return(state);
  }

  static int Meassure::getAirCondition() {
    return(airCondition);
  }

  static int Meassure::getLowest() {
    return(lowest);
  }

  static int Meassure::getPpmSinceVentilation() {
    return(ppmSinceVentilation);
  }

  static unsigned long Meassure::getStartTime() {
    return(startTime);
  }

  static void Meassure::debug() {
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

  static void Meassure::meassureAirCondition() {
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

  static void Meassure::mapAirCondition() {
    //to PPM
    if (airCondition <= Util::calibration[EEPROM.read(0)].getLowestSensor())
      airCondition = Util::calibration[EEPROM.read(0)].getLowestSensor();

    airCondition = map(airCondition, Util::calibration[EEPROM.read(0)].getLowestSensor(), Util::calibration[EEPROM.read(0)].getHighestSensor(), Util::calibration[EEPROM.read(0)].getLowestPPM(), Util::calibration[EEPROM.read(0)].getHighestPPM());

  }


  static void Meassure::calculateGradient() {
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
    gradient = now / last;
  }

  static void Meassure::checkVentilating() {
    //start Ventilating
    if (gradient < MAX_DECREASE && state != VENTILATING) { // Wenn die Differenz die Hemmschwelle übersteigt: Wird Ventilating erkannt
      state = VENTILATING;
      ppmSinceVentilation = airCondition;
      Util::rgb(0, 0, 255);
      startTime = millis();
    }

    //stop Ventilating
    if ((gradient > 1 && state == VENTILATING) || (millis() - timer >= VENTILATING_TIMEOUT && state == VENTILATING && timer != 0)) {
      // Wenn der Graph nach oben Steigt oder der Timer abgelaufen ist
      if (airCondition - airConditionLowest < MAX_INCREASE_LOWEST) {
        //Wenn
        airConditionLowest = ALPHA_LOWEST * airCondition + (1 - ALPHA_LOWEST) * airConditionLowest;
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

  static void Meassure::setState() {

    // define colorState  -changed
    if(airCondition < LIMIT_GOOD) {
      colorState = 0;
    } else if (airCondition < LIMIT_MEDIUM) {
      colorState = 1;
    } else if (airCondition < LIMIT_BAD) {
      colorState = 2;
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
      state = -1;
    if (state > 5)
      state = 5;
  }

  static void Meassure::writeLed() {
    //map Values
    led = map(airCondition, lowest, LIMIT_ALARM, 0, 255);
    red = led;
    green = 255 - led;

    //turn on
    if (state != VENTILATING)
      Util::rgb(red, green, 0);
  }
