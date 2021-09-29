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

  Adafruit_BME280 Meassure::bme = Adafruit_BME280(BMESDA, BMESCL);
  MHZ19 Meassure::MHZ19b;
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
    if (bme.begin(0x76)) {
      debug(INFO, SETUP, "BME-Sensor initialized");
    } else {
      debug(WARNING, SETUP, "Could not find a valid BME280 sensor, check wiring!");
    }
    Serial1.begin(MHZ19BAUDRATE);
    MHZ19b.begin(Serial1);
    while (!SENSORCONNECTED && requestDecision("Sensor nicht verbunden", "erneut versuchen?", "Ja", "Nein", 15000, true)) {
      drawLogo();
      Serial.print("connecting to sensor");
      for (int x = 0; x <= 10; x++) {
        delay(500);
        Serial.print(".");
      }
      Serial.println();
      MHZ19b.begin(Serial1);     
    }
    drawLogo();
    if(SENSORCONNECTED) {
      MHZ19b.autoCalibration(false);
      debug(INFO, SENSOR, "ABC Status: " + MHZ19b.getABC() ? "ON" : "OFF");  // now print it's status
      // MHZ19b.setRange(RANGE);
      // MHZ19b.calibrate();
      // MHZ19b.zeroSpan(1000);
    }

    if(SENSORCONNECTED) {
      for(int x = 0; x <= 180 && ((MHZ19b.getCO2(true, true) == 10000) || (MHZ19b.getCO2(true, true) <= 0)); x++) {
        delay(1000);
      }
      delay(SENSORDROPTIME);
    }

    startTime = millis();
    debug(DEBUG, SETUP, "Meassure SETUP ended");
    debug(DEBUG, SETUP, "");

  }

  void Meassure::loop() {
    // meassureEnvironment();
    if(meassureAirCondition()) {
      calculateGradient();
      checkVentilating();
      setState();
      debugMeassure();
    }
  }

  void Meassure::reconnect() {
    MHZ19b.begin(Serial1);
  }

  boolean Meassure::isConnected() {
    return SENSORCONNECTED;
  }

  void Meassure::calibrateMin() {
    if(requestDecision("Min Wert Kalibrierung", "Möchtest du fortfahren?")) {
      MHZ19b.calibrate();
      debug(WARNING, SENSOR, "min value calibrated to 400 PPM");
    }
    Menu::setup();
  }

  void Meassure::calibrateMax() {
    if(requestDecision("Max Wert Kalibrierung", "Möchtest du fortfahren?")) {
      MHZ19b.zeroSpan(1000);
      debug(WARNING, SENSOR, "max value calibrated to 1000 PPM");
    }
    Menu::setup();
  }

  void Meassure::autoCalibrationMin() {
    int average[30];
    if(requestDecision("Auto Min Wert", "Möchtest du fortfahren?")) {
      int counter = 1;
      int value = 400;
      while(true/* requestDecision("Round" + counter, "Möchtest du fortfahren?") */) {
        Serial.println("AutoMinCali: Round" + (String) counter);
        display.fillScreen(BLACK);
        dPrint("Auto Min Cali", 20, 0, 3, CYAN);
        dPrint("Round " + counter, 20, 40, 3, BLUE);
        for(int x = 0; x < 30; x++) {
          value = MHZ19b.getCO2(true, true);
          average[x] = value;
          if((value < 600) && (value > 200) && (x != 0)) {
            average[x] = average[x-1];
          }
          display.fillRect(10, 80, 80, 80, BLACK);
          dPrint(average[x], 20, 80, 3, LIME);
          Serial.print((String) value + ", ");
          delay(1000);
        }
        Serial.println();
        int output = 0;
        for(int x = 0; x < 30; x++) {
          output += average[x];
        }
        output = output / 30;
        if(output < 400) {
          if(requestDecision("Average" + (String) output, "Möchtest du 0 punkt kallibrieren?")) {
            MHZ19b.calibrate();
            Serial.println("Auto callibrated Min Value");
          }
        }
        counter++;

      }
    }
  }

  //Getter
  State Meassure::getState() {
    return(state);
  }

  int Meassure::getAirCondition() {
    return((int) airCondition);
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
    // if(SENSORCONNECTED) {
      counter++;
      if(counter >= AVERAGING_MEASUREMENTS) {
        airCondition = MHZ19b.getCO2(true, true);
        //Additional Mapping for bad sensors
        if((SENSORMAPMIN > 0) && (SENSORMAPMAX > 0)) {
          airCondition = map(airCondition, 400, 2000, SENSORMAPMIN, SENSORMAPMAX);
        }

        debug(SPAMM, SENSOR, "PPM: " + String(airCondition));
        //Serial.println(airCondition);
        //Serial.println(SENSORCONNECTED);
        /*Serial.print(MHZ19b.getCO2Raw(true)); 
        Serial.print("  ");
        Serial.print(MHZ19b.getCO2(false, true));
        Serial.print("  ");
        Serial.print(MHZ19b.getCO2(true, true));
        Serial.print("  ");
        Serial.print(MHZ19b.getTemperature(false, true));
        Serial.print("  ");
        Serial.print(MHZ19b.getTemperature(true, true));
        Serial.print("  ");
        Serial.print(MHZ19b.getTempAdjustment());
        Serial.println();
        temperature = MHZ19b.getTemperature(true, false);*/
        //Serial.println(temperature);


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
    // }

    return false;
  }

  void Meassure::meassureEnvironment() {
    temperature = bme.readTemperature();
    humidity = bme.readHumidity();
    pressure = bme.readPressure()/100;
    // debug(INFO, SENSOR, "Temperature: " + temperature + String(char(167)) +"C " + "Humidity: " + humidity + "% " + "Pressure: " + pressure + "mbar");
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

  MHZ19 Meassure::getSensor() {
    return Meassure::MHZ19b;
  }