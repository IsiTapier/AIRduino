/*
 Manager.cpp - Manager-Library for Librarys.
*/

#include "Manager.h"

#define CURRENTCYCLE floor((Manager::currentCycleTime-Meassure::getStartTime())/STAGE_TIME)
#define LASTCYCLE floor((Manager::lastCycleTime-Meassure::getStartTime())/STAGE_TIME)
#define STAGE_TIME_2 300

using namespace general;

  unsigned long Manager::lastCycleTime = 0;
  unsigned long Manager::lastCycleTime2 = 0;
  unsigned long Manager::lastLoop = 0;
  unsigned long Manager::lastReconnect = 0;
  unsigned long Manager::lastModeChange = 0;
  unsigned long Manager::currentCycleTime = 0;
  unsigned long Manager::currentCycleTime2 = 0;


void Manager::setup() {
    mode.setValue(LOADINGSCREEN, false);
    Display::setup();
    lastReconnect = millis();
    eeprom(); 
    xTaskCreate(backgroundLoop, "backgroundLoop", 4096, NULL, 0, NULL);  
    
    Meassure::setup();
    DecibelGui::setup();
    Serial.println(xPortGetCoreID());
    mode.setValue(LOADINGSCREEN, false);
    mode.setValue(CHART); 
}

void Manager::loop() {
    //Timing System Check
    currentCycleTime = millis();

    if(currentCycleTime - lastCycleTime > STAGE_TIME) {
       /*  Serial.print("Warning - timing system failed: ");
        Serial.print(currentCycleTime-lastCycleTime-STAGE_TIME);
        Serial.println("ms"); */
      }
    lastCycleTime = currentCycleTime;

    // Display loop
    Display::loop();
    
    //guis
    if(mode.getValue() == CHART) {
      TimerGui::loop();
      StopwatchGui::loop();
      OverviewGui::loop();
      DecibelGui::loop();
      CalibrateGui::loop();
      MenuGui::loop();
    }
    if(500 >= (TimerGui::goalMillis - millis())) {
       TimerGui::resetTimer();
    }
    TimerGui::peep();
}

void Manager::backgroundLoop(void* parm) {
  setupDatabaseConnection();
  for(;;) {  
    reconnectSystem();
    if(client.connected()) {
      for(int x = 0; x <= 50; x++) {
         client.loop();
         vTaskDelay(10/portTICK_PERIOD_MS);
      } 
    }
    // Serial.println("test");
    vTaskDelay(RECONNECT_TIME/portTICK_PERIOD_MS);
    
  } 
}



void Manager::eeprom() {
    EEPROM.begin(EEPROM_SIZE);
    if(EEPROM.read(0) == 255) {
      EEPROM.write(0, 0);
      EEPROM.commit();
    }
    // ts.calibration();
    if(EEPROM.readShort(XMIN) == EEPROM.readShort(XMAX) || EEPROM.readShort(YMIN) == EEPROM.readShort(YMAX))
      ts.calibration();
    if(debugSetup.getValue() && debugPriority.getValue()) {
      debug(INFO, SETUP, "EEPROM: sensor", EEPROM.read(0));
      debug(INFO, SETUP, "EEPROM: xmin", EEPROM.readShort(XMIN));
      debug(INFO, SETUP, "EEPROM: xmax", EEPROM.readShort(XMAX));
      debug(INFO, SETUP, "EEPROM: ymin", EEPROM.readShort(YMIN));
      debug(INFO, SETUP, "EEPORM: ymax", EEPROM.readShort(YMAX));
    }
    developper::isMappingActive.setValue(GET_MAP_IS_ACTIVE);
  }

  //    RESET EEPROM    //
    /*EEPROM.write(0, 0);
    EEPROM.writeShort(XMIN, 0);
    EEPROM.writeShort(XMAX, 0);
    EEPROM.writeShort(YMIN, 0);
    EEPROM.writeShort(YMAX, 0);
    EEPROM.commit();*/
