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

  // extern MQ135 Meassure::sensor = MQ135(GAS_SENSOR);
  extern Adafruit_BME280 Meassure::bme = Adafruit_BME280(BMESDA, BMESCL);
  extern MHZ19 Meassure::MHZ19b;
  extern unsigned long Meassure::tempAirCondition;
  extern unsigned long Meassure::temptempAirCondition;
  extern float Meassure::airConditionTemp;
  extern float Meassure::airCondition;
  extern float Meassure::airConditionRaw;
  extern float Meassure::airConditionLast;
  extern unsigned long Meassure::startTime;
  extern unsigned long Meassure::timer;
  extern int Meassure::gradient;
  extern int Meassure::value;
  extern int Meassure::values[AVERAGING_GRADIENT * 2];
  extern int Meassure::now;
  extern int Meassure::last;
  extern int Meassure::minPPM = 400;
  extern int Meassure::maxPPM;
  extern State Meassure::state;
  extern int Meassure::colorState;
  extern int Meassure::temperature;
  extern int Meassure::humidity;
  extern int Meassure::pressure;

  extern int Meassure::testCounter = 0;
  extern unsigned long Meassure::lasttime;

  void Meassure::setup() {
    debug(DEBUG, SETUP, "Meassure SETUP started");
    pinMode(PIEZO, OUTPUT);
    // pinMode(SENSOR, INPUT);
    digitalWrite(PIEZO, LOW);
    debug(INFO, SETUP, "Pins initialized");
    // airConditionRaw = analogRead(GAS_SENSOR);
    // airConditionLast = analogRead(GAS_SENSOR);
    startTime = millis();
    debug(INFO, SETUP, "Variables initialized");
    if (bme.begin(0x76)) {
      debug(INFO, SETUP, "BME-Sensor initialized");
    } else {
      debug(WARNING, SETUP, "Could not find a valid BME280 sensor, check wiring!");
    }
    Serial1.begin(MHZ19BAUDRATE);
    MHZ19b.begin(Serial1);
    MHZ19b.autoCalibration(false);
    debug(INFO, SENSOR, "ABC Status: " + MHZ19b.getABC() ? "ON" : "OFF");  // now print it's status
    // MHZ19b.setRange(RANGE);
    // MHZ19b.calibrate();
    // MHZ19b.zeroSpan(1000);
    debug(DEBUG, SETUP, "Meassure SETUP ended");
    debug(DEBUG, SETUP, "");
  }

  void Meassure::loop() {
    meassureEnvironment();
    meassureAirCondition();
    mapAirCondition();
    calculateGradient();
    checkVentilating();
    setState();
    debugMeassure();
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
/*    if (general::debugSensor.getValue() && general::debugPriority.getValue()) {
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

  //  __  __
  // |  \/  |
  // | \  / | ___  __ _ ___ ___ _   _ _ __ ___
  // | |\/| |/ _ \/ _` / __/ __| | | | '__/ _ \
  // | |  | |  __/ (_| \__ \__ \ |_| | | |  __/
  // |_|  |_|\___|\__,_|___/___/\__,_|_|  \___|

  void Meassure::meassureAirCondition() {
    //Messung
    int timeLeft = STAGE_TIME-(millis()%STAGE_TIME);
    if(timeLeft < 0.8*STAGE_TIME) {
      timeLeft += STAGE_TIME;
      //Serial.println("WARNING: Timing error!");
    }
    timeLeft-=10;

    /*tempAirCondition = 0;
    temptempAirCondition = 0;
    for (long i = 0; i < AVERAGING_MEASUREMENTS; i++) {
      // value = analogRead(GAS_SENSOR);

      //Fehlmessungen überschreiben
    /*  if (airConditionRaw * MAX_INCREASE < value && i == 0)
        tempAirCondition = tempAirCondition + airConditionRaw;
      else if (airCondition / i * MAX_INCREASE < value && i != 0)
        tempAirCondition = tempAirCondition + tempAirCondition / i;
      else*/
    /*    tempAirCondition = tempAirCondition + value;
        temptempAirCondition += analogRead(39);
      delay(timeLeft/ AVERAGING_MEASUREMENTS);
    }

    airCondition = (float) tempAirCondition / AVERAGING_MEASUREMENTS;
    airConditionTemp = (float) temptempAirCondition / AVERAGING_MEASUREMENTS;
    airConditionRaw = airCondition;*/

    airCondition = MHZ19b.getCO2(true, true);
    Serial.println(airCondition);
    temperature = MHZ19b.getTemperature(true, true);

    //Wert smoothen;
    //airCondition = ALPHA_MEASUREMENTS * airCondition + (1 - ALPHA_MEASUREMENTS) * airConditionLast;

    airConditionLast = airCondition;
//    airCondition = sensor.getPPM(temperature, humidity);
    
    int time = floor(millis()/1000);
    if(lasttime != time) {
      //Serial.println(testCounter);
      testCounter = 0;
    }
    testCounter++;
    lasttime = time;

    while(millis()%STAGE_TIME > 0) {}
  }

  void Meassure::meassureEnvironment() {
    temperature = bme.readTemperature();
    humidity = bme.readHumidity();
    pressure = bme.readPressure()/100;
    debug(INFO, SENSOR, "Temperature: " + temperature + String(char(167)) +"C " + "Humidity: " + humidity + "% " + "Pressure: " + pressure + "mbar");
  }

  //  _____        _
  // |  __ \      | |
  // | |  | | __ _| |_ __ _
  // | |  | |/ _` | __/ _` |
  // | |__| | (_| | || (_| |
  // |_____/ \__,_|\__\__,_|


  void Meassure::mapAirCondition() {
    //to PPM
  /*  if (airCondition <= calibration[EEPROM.read(0)].getLowestSensor())
      airCondition = calibration[EEPROM.read(0)].getLowestSensor();*/
  /*  airCondition = map(airCondition*100, calibration[EEPROM.read(0)].getLowestSensor()*100, calibration[EEPROM.read(0)].getHighestSensor()*100, calibration[EEPROM.read(0)].getLowestPPM(), calibration[EEPROM.read(0)].getHighestPPM());
    airCondition += 400 - minPPM;*/
    //sd.saveValuesToSD(millis()/1000, airConditionRaw, airConditionLast, airCondition);
    //Serial.print(airCondition); Serial.print("\t");
    //Serial.print(0); Serial.print("\t");
  /*  Serial.print(airConditionTemp); Serial.print("\t");
    Serial.println(0);*/
    //Serial.print(temperature); Serial.print("\t");
    //Serial.println(humidity); /*Serial.print("\t");
  /*  Serial.print(sensor.getRZero(airConditionRaw)); Serial.print("\t");
    Serial.print(sensor.getRZero(airConditionRaw, 1)); Serial.print("\t");
    Serial.print(sensor.getRZero(airConditionRaw, "1")); Serial.print("\t");
    Serial.print(sensor.getRZero(airConditionRaw, temperature, humidity)); Serial.print("\t");
    Serial.print(sensor.getRZero(airConditionRaw, temperature, humidity, 1)); Serial.print("\t");
    Serial.println(sensor.getRZero(airConditionRaw, temperature, humidity, "1"));*/
    mysql_insert(device_class, airCondition, temperature);
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
    gradient = now - last;
  }

  void Meassure::checkVentilating() {
    //start Ventilating
    if (gradient < MAX_DECREASE && state != VENTILATING) { // Wenn die Differenz die Hemmschwelle übersteigt: Wird Ventilating erkannt
      state = (State) VENTILATING;
      maxPPM = airCondition;
      startTime = millis();
      debug(DEBUG, SENSOR, "VENTILATING");
    }

    //stop Ventilating
    else if ((gradient > MAX_INCREASE && state == VENTILATING) || (millis() - timer >= VENTILATING_TIMEOUT && state == VENTILATING && timer != 0)) {
      // Wenn der Graph nach oben Steigt oder der Timer abgelaufen ist
      debug(DEBUG, SENSOR, "NOT VENTILATING");
      if (airCondition - minPPM < MAX_INCREASE_LOWEST && gradient < 3) {
        //Wenn
        minPPM = ALPHA_LOWEST * airCondition + (1 - ALPHA_LOWEST) * minPPM;
      }

      state = (State) (States) 0;
      timer = 0;
      startTime = millis();


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
    if(state != -1) {
      state = getStateOf(airCondition);
    }

    //colorState = map(airCondition, calibration[EEPROM.read(0)].getLowestPPM(), calibration[EEPROM.read(0)].getHighestPPM(), 0, 2);
/*
    if (state == -1)
      return;

    if (airCondition > LIMIT_PIEP)
      state = PIEP;
    else if (airCondition > LIMIT_BLINK)
      state = BLINK;

    if (state < -1)
      state =  -1;
    if (state > 4)
      state =  4;*/
  }
