/*
 Display.cpp - Display-Library for Librarys.
*/

#include "Display.h"

using namespace general;

  TSPoint Display::p;
  unsigned long Display::lastCycleTime = 0;
  unsigned long Display::lastLoop = 0;
  unsigned long Display::lastReconnect = 0;
  unsigned long Display::lastModeChange = 0;
  unsigned long Display::lastTouch;

  void Display::setup() {
    debug(DEBUG, SETUP, "Display SETUP started");
    mode.setValue(LOADINGSCREEN, false);
    display.begin();
    debug(INFO, SETUP, "Display connection started");
    display.fillScreen(BACKGROUND_COLOR);
    display.setTextWrap(false);
    display.setRotation(ROTATION);
    debug(INFO, SETUP, "Display initialized");
    //Logo
    drawLogo();
    eeprom();
    /*unsigned long begin = millis();
    while(millis() - begin > 2000) {
      p = ts.getPoint();
      if(p.isTouching()) {
        ts.calibration();
        break;
      }
    }*/
    setupDatabaseConnection();
    mode.setValue(LOADINGSCREEN, false);
    mode.setValue(CHART);
    debug(DEBUG, SETUP, "Display SETUP completed");
    debug(DEBUG, SETUP, "");
    //drawInfoScreen(5000);
    Meassure::setup();
    drawLogo();
    lastReconnect = millis();
  }

  void Display::eeprom() {
    EEPROM.begin(EEPROM_SIZE);
    //    RESET EEPROM    //
    /*EEPROM.write(0, 0);
    EEPROM.writeShort(XMIN, 0);
    EEPROM.writeShort(XMAX, 0);
    EEPROM.writeShort(YMIN, 0);
    EEPROM.writeShort(YMAX, 0);
    EEPROM.commit();*/
    if(EEPROM.read(0) == 255) {
      EEPROM.write(0, 0);
      EEPROM.commit();
    }
    if(EEPROM.readShort(XMIN) == EEPROM.readShort(XMAX) || EEPROM.readShort(YMIN) == EEPROM.readShort(YMAX))
      ts.calibration();
    if(debugSetup.getValue() && debugPriority.getValue()) {
      debug(INFO, SETUP, "EEPROM: sensor", EEPROM.read(0));
      debug(INFO, SETUP, "EEPROM: xmin", EEPROM.readShort(XMIN));
      debug(INFO, SETUP, "EEPROM: xmax", EEPROM.readShort(XMAX));
      debug(INFO, SETUP, "EEPROM: ymin", EEPROM.readShort(YMIN));
      debug(INFO, SETUP, "EEPORM: ymax", EEPROM.readShort(YMAX));
    }
  }

  void Display::loop() {
    unsigned long currentCycleTime = millis();
    while(currentCycleTime - lastCycleTime < STAGE_TIME) {currentCycleTime = millis();}
    if(currentCycleTime - lastCycleTime > STAGE_TIME) {
      Serial.print("Warning - timing system failed: ");
      Serial.print(currentCycleTime-lastCycleTime-STAGE_TIME);
      Serial.println("ms");
    }
    lastCycleTime = currentCycleTime;
    if(client.connected())
      client.loop();
    handleTouch();
    initDisplay();
    if(currentCycleTime - lastReconnect >= RECONNECT_TIME) {
      lastReconnect = currentCycleTime;
      reconnect();
    }
    if(currentCycleTime - lastLoop >= LOOP_TIME) {
      lastLoop = currentCycleTime;

      Meassure::loop();
      boolean changed = DisplayV1::getGraphData();
      /*if(mode.getValue() == MENU) {
        Menu::loop();
      } else */

      if(mode.getValue() == MENU) {
        Menu::loop();
      } else if(mode.getValue() == CHART) {
        if(version.equals(V1)) {
          DisplayV1::loop(changed);
        } else if(version.equals(V2)) {
          DisplayV2::loop();
        } else if(version.equals(V3)) {
          DisplayV3::loop();
        }
      } else if(mode.getValue() == MAINTENANCE) {
        maintenanceMode();
      }
    }
  }

  void Display::initDisplay() {
    if(mode.hasChanged() || (version.hasChanged() && !mode.equals(MENU)) || theme.hasChanged() || language.hasChanged()) {
      if(mode.getValue() == CHART) {
        if(version.equals(V1)) {
          DisplayV1::setup();
          DisplayV1::loop(false);
        } else if(version.equals(V2)) {
          DisplayV2::setup();
          DisplayV2::loop();
        } else if(version.equals(V3)) {
          DisplayV3::setup();
          DisplayV3::loop();
        }
      } else if(mode.getValue() == MENU && (theme.hasChanged() || mode.hasChanged() || language.hasChanged())) {
        ledcDetachPin(PIEZO);
        Menu::setup();
      } else if(mode.getValue() == LOADINGSCREEN) {
        Serial.println("Restart");
        ESP.restart();
      } else if(mode.getValue() == RESET) {
        mode.setValue(mode.getOldValue());
        Menu::reset();
        initDisplay();
      }
      version.setValue(version.getValue(), false);
      theme.setValue(theme.getValue(), false);
      language.setValue(language.getValue(), false);
      mode.setValue(mode.getValue(), false);
    }
  }

  void Display::handleTouch() {
    if(!p.isTouching()) {
      // a point object holds x y and z coordinates
      p = ts.getPoint();
      // we have some minimum pressure we consider 'valid'
      // pressure of 0 means no pressing!
      //template:: p.isTouching(PLACEHOLDER_PLACE_START_X, PLACEHOLDER_PLACE_END_X, PLACEHOLDER_PLACE_START_Y, PLACEHOLDER_PLACE_END_Y)
      if(p.isTouching() && millis()-lastTouch >= TOUCH_COOLDOWN && millis() > lastTouch) {
        p.calibrate();
        if(!p.isTouching())
          return;
        lastTouch = millis();
        if(p.isTouching(MENU_ARROW_BACK_START_X-3, MENU_ARROW_BACK_END_X+3, MENU_ARROW_BACK_START_Y-3, MENU_ARROW_BACK_END_Y+3)) {
          if(debugSetup.getValue())
            debug(INFO, SETUP, "change Mode");
          if(mode.equals(MENU))
            mode.setValue(CHART);
          else if(mode.equals(CHART))
            mode.setValue(MENU);
          lastModeChange = millis();
          lastTouch+=500;
        } else if(p.isTouching(MENU_ARROW_RESET_START_X-3, MENU_ARROW_RESET_END_X+3, MENU_ARROW_RESET_START_Y-3, MENU_ARROW_RESET_END_Y+3)) {
          if(mode.equals(MENU) && requestDecision("Einstellungs Reset", "Willst du fortfahren?")) {
            if(debugSetup.getValue())
              debug(WARNING, SETUP, "reset");
            Menu::reset();
            Menu::setup();
          } else if(mode.equals(CHART)) {
            Meassure::resetStartTime(true);
            DisplayV1::resetGraph();
            if(version.equals(V1))
              DisplayV1::setup();
          }
        } else if(mode.equals(MENU)) {
          Menu::handleTouch(p);
        } else if(mode.equals(CHART)) {
          version.shiftValue();
          lastTouch+=500;
        } 
      }
    } else {
      p = ts.getPoint();
    }
  }

void Display::drawInfoScreen(int time) {
  display.fillScreen(BLACK);


  dPrint(device_class, 160, 10, 5, LIGHT_BLUE, 1);
  dPrint("Software:", 200, 70, 3, GREY, 5);
  dPrint(software_version, 200, 70, 3, LIGHTGREY, 3);

  dPrint("Mapping:", 200, 100, 3, GREY, 5);
  if((SENSORMAPMIN == 0) && (SENSORMAPMAX == 0)) {
    dPrint("AUS", 200, 100, 3, RED, 3);
  } else {
  	dPrint("AN", 200, 100, 3, GREEN, 3);
  }


  dPrint("Connection:", 200, 130, 3, GREY, 5);
  if(client.connected()) {
     dPrint("online", 200, 130, 3, GREEN, 3);
  } else {
    dPrint("MQTT", 200, 130, 3, RED, 3);
  }   

  if(WL_CONNECTED) {
    dPrint("online", 200, 130, 3, GREEN, 3);
  } else {
    dPrint("WLAN", 200, 130, 3, RED, 3);
  }

  delay(time);
  general::mode.setValue(LOADINGSCREEN);
  general::mode.setValue(general::mode.getOldValue());  
}

void Display::reconnect() {
  boolean isConnected = WiFi.isConnected();
  Serial.print("Wifi: "); Serial.println(isConnected?"connected":"unconnected");
  if(!isConnected) {
    reconnectToWifi();
    Serial.print("reconnect "); Serial.println(WiFi.isConnected()?"successful":"failed");
  }
  isConnected = client.connected();
  Serial.print("MQTT: "); Serial.println(isConnected?"connected":"unconnected");
  if(!isConnected) {
    reconnectToMQTT();
    Serial.print("reconnect "); Serial.println(client.connected()?"successful":"failed");
  }
  isConnected = Meassure::isConnected();
  Serial.print("Sensor: "); Serial.println(isConnected?"connected":"unconnected");
  if(!isConnected) {
    Meassure::reconnect();
    Serial.print("reconnect "); Serial.println(Meassure::isConnected()?"successful":"failed");
  }
}

