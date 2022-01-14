/*
  Meassure.cpp - Meassure-Library for Librarys.
*/

#include "Arduino.h"
#include "Meassure.h"
#include "../display/menu/Menu.h"

  /*
     _____      _
    / ____|    | |
   | (___   ___| |_ _   _ _ __
    \___ \ / _ \ __| | | | '_ \
    ____) |  __/ |_| |_| | |_) |
   |_____/ \___|\__|\__,_| .__/
                         | |
                         |_|
  */

  // Adafruit_BME280 Meassure::bme = Adafruit_BME280(BMESDA, BMESCL);
  // MHZ19 Meassure::MHZ19b;
  MHZ Meassure::MHZ19;
  unsigned long Meassure::tempAirCondition;
  unsigned long Meassure::temptempAirCondition;
  float Meassure::airConditionTemp;
  float Meassure::airCondition = 0;
  float Meassure::airConditionRaw;
  float Meassure::airConditionLast;
  unsigned long Meassure::startTime;
  unsigned long Meassure::timer;
  int Meassure::gradient;
  int Meassure::value;
  int Meassure::values[AVERAGING_GRADIENT * 2];
  int Meassure::counter = AVERAGING_MEASUREMENTS;
  int Meassure::now;
  int Meassure::last;
  int Meassure::minPPM = 400;
  int Meassure::maxPPM;
  State Meassure::state;
  int Meassure::colorState;
  int Meassure::temperature;
  int Meassure::humidity;
  int Meassure::pressure;
  int Meassure::decibelValue = 0;

  int Meassure::testCounter = 0;
  unsigned long Meassure::lasttime;

  void Meassure::setup() {
    debug(DEBUG, SETUP, "Meassure SETUP started");
    debug(INFO, SETUP, "Pins initialized");
    debug(INFO, SETUP, "Variables initialized");
    // }
    drawLogo();
    // if(SENSORCONNECTED) {
    //   MHZ19b.autoCalibration(false);
    //   debug(INFO, SENSOR, "ABC Status: " + MHZ19b.getABC() ? "ON" : "OFF");  // now print it's status
    //   // MHZ19b.setRange(RANGE);
    //   // MHZ19b.calibrate();
    //   // MHZ19b.zeroSpan(1000);
    // }

    // if(SENSORCONNECTED) {
    //   for(int x = 0; x <= 180 && ((MHZ19b.getCO2(true, true) == 10000) || (MHZ19b.getCO2(true, true) <= 0)); x++) {
    //     delay(1000);
    //   }
    //   delay(SENSORDROPTIME);
    // }
    MHZ19.setDebug(true);
    Serial2.begin(MHZ19BAUDRATE, 134217756U, 22, 23);
    MHZ19 = MHZ(&Serial2, SENSOR_TYPE);
    // MHZ19.setDebug(true);
    MHZ19.setAutoCalibrate(false);
    if (MHZ19.isPreHeating()) {
      Serial.println("SENSOR: Preheating");
      /* while (MHZ19.isPreHeating()) {
        // Serial.print(".");
        delay(1000);
      }
      Serial.println("SENSOR: Preheating ended..."); */
    }
    MHZ19.setAutoCalibrate(false);
    startTime = millis();
    debug(DEBUG, SETUP, "Meassure SETUP ended");
    debug(DEBUG, SETUP, "");
  }

  void Meassure::loop() {
    // meassureEnvironment();
    reconnect();
    if(meassureAirCondition()) {
      calculateGradient();
      checkVentilating();
      setState();
      debugMeassure();
    }
  }

  void Meassure::reconnect() {
    // if(SENSORCONNECTED) return;
    // MHZ19b.begin(Serial1);
    // MHZ19b.autoCalibration(false);
  }

  boolean Meassure::isConnected() {
    return SENSORCONNECTED;
  }

  void Meassure::calibrateMin() {
    if(requestDecision("Min Wert Kalibrierung", "Möchtest du fortfahren?")) {
      int difference = airCondition - 400;
      // MHZ19b.calibrate();
      MHZ19.calibrateZero();
      SET_MAP_MAX_IN(GET_MAP_MAX_IN - difference);
      SET_MAP_MAX_OUT(GET_MAP_MAX_OUT - difference);
      debug(WARNING, SENSOR, "min value calibrated to 400 PPM");
    }
    Menu::setup();
  }

  void Meassure::forcedMinCalibration() {
    // MHZ19b.calibrate();
    MHZ19.calibrateZero();
    Serial.println("Calibrated Min");
  }

  void Meassure::calibrateMax(int targetPPM) {
    if(requestDecision("kunst. Kalibrierung " + targetPPM, "Möchtest du fortfahren?")) {
      // MHZ19b.zeroSpan(1000);
      SET_MAP_MAX_IN((short) MHZ19.readCO2UART()/*MHZ19b.getCO2(true, true)*/);
      SET_MAP_MAX_OUT((short) targetPPM);
      SET_MAP_IS_ACTIVE(true);
      developper::isMappingActive.setValue(true);
      Serial.print("Calibrated MAX Value to "); Serial.println(targetPPM);
      Serial.println(GET_MAP_MAX_IN);
      Serial.println(GET_MAP_MAX_OUT);
      Serial.println(GET_MAP_IS_ACTIVE);
      
    }
    Menu::setup();
  }

  //Getter
  State Meassure::getState() {
    return(state);
  }

  int Meassure::getAirCondition() {
    return((int) airCondition);
  }

  int Meassure::getRawAirCondition() {
    return (int) airConditionRaw;
  }

  int Meassure::getLowest() {
    return(minPPM);
  }

  int Meassure::getHighest() {
    return(maxPPM);
  }

  unsigned long Meassure::getStartTime() {
    return(startTime);
  }
  

  void Meassure::debugMeassure() {
/*  if (general::debugSensor.getValue() && general::debugPriority.getValue()) {
      Serial.println("");
      Serial.println("Sensor");
      Serial.println("");
      debug("Analog", analogRead(GAS_SENSOR));
      debug("Average", airConditionRaw);
      debug("Smoothed", airConditionLast);
      debug("PPM", airCondition);
      debug("gradient", gradient);
      debug("min PPM", minPPM);
      debug("max PPM", maxPPM);
      debug("Status", (int) state);
    }*/
  }

  /*
    __  __
   |  \/  |
   | \  / | ___  __ _ ___ ___ _   _ _ __ ___
   | |\/| |/ _ \/ _` / __/ __| | | | '__/ _ \
   | |  | |  __/ (_| \__ \__ \ |_| | | |  __/
   |_|  |_|\___|\__,_|___/___/\__,_|_|  \___|

  */

  boolean Meassure::meassureAirCondition() {
    if(SENSORCONNECTED) {
      counter++;
      if(counter >= AVERAGING_MEASUREMENTS) {
        airCondition = MHZ19.readCO2UART();//MHZ19b.getCO2(true, true);
        airConditionRaw = airCondition;
        //Additional Mapping for bad sensors
        if(developper::isMappingActive.getValue() == 1) {
          airCondition = map(airCondition, 400, GET_MAP_MAX_IN, 400, GET_MAP_MAX_OUT);
        }
        temperature = MHZ19.getLastTemperature();
        debug(ERROR, SETUP, "PPM: " + String(airCondition));
        debug(ERROR, SETUP, "Temperature: " + String(temperature));

        //Wert smoothen;
        //airCondition = ALPHA_MEASUREMENTS * airCondition + (1 - ALPHA_MEASUREMENTS) * airConditionLast;

        if(SENSORERROR && !SENSORERRORLAST) {
          airCondition = airConditionLast;
          airConditionLast = 0;
        } else
          airConditionLast = airCondition;
        if(airCondition != 0 /*|| temperature != 0*/)
          mysql_insert(device_class, airCondition, temperature, decibelValue);
        counter = 0;
        return true;
      }
      return false;
    }
    return false;
  }
  //  _____        _
  // |  __ \      | |
  // | |  | | __ _| |_ __ _
  // | |  | |/ _` | __/ _` |
  // | |__| | (_| | || (_| |
  // |_____/ \__,_|\__\__,_|

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
    gradient = now - last;
  }

  void Meassure::checkVentilating() {
    //start Ventilating
    if (gradient < MAX_DECREASE && state != VENTILATING) { // Wenn die Differenz die Hemmschwelle übersteigt: Wird Ventilating erkannt
      state = (State) VENTILATING;
      maxPPM = airCondition;
      if(general::autoTimeReset.getValue()) {
        startTime = millis();
        counter = 0;
      }
      debug(INFO, SENSOR, "VENTILATING");
    }

    //stop Ventilating
    else if ((gradient > MAX_INCREASE && state == VENTILATING) || (millis() - timer >= VENTILATING_TIMEOUT && state == VENTILATING && timer != 0)) {
      // Wenn der Graph nach oben Steigt oder der Timer abgelaufen ist
      debug(INFO, SENSOR, "NOT VENTILATING");
      if (airCondition - minPPM < MAX_INCREASE_LOWEST && gradient < 3) {
        //Wenn
        minPPM = ALPHA_LOWEST * airCondition + (1 - ALPHA_LOWEST) * minPPM;
      }
      state = (State) (States) 0;
      timer = 0;
      if(general::autoTimeReset.getValue()) {
        startTime = millis();
        counter = 0;
      }
    } else if (gradient == 0 && timer == 0 && state == VENTILATING) {
      //Wenn der Graph nicht mehr steigt und der Timer noch nicht gestartet wurde
      //Ziel nach gewisser Zeit kein Abstieg -> Timer für Timeout wird gestartet
      timer = millis();
    } else if (gradient != 0) {
      timer = 0;
    }
    last = now;
  }

  void Meassure::setState() {
    // define colorState  -changed
    if(state != -1)
      state = getStateOf(airCondition);
  }

  void Meassure::resetStartTime(boolean cycle) {
    startTime = millis();
    if(cycle)
    counter = -1;
  }

  MHZ Meassure::getSensor() {
    return Meassure::MHZ19;
  }