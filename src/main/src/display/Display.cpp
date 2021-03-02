/*
 Display.cpp - Display-Library for Librarys.
*/

#include "Display.h"

using namespace general;

  extern TSPoint Display::p;
  extern unsigned long Display::lastModeChange = 0;

  void Display::setup() {
    debug(DEBUG, SETUP, "Display SETUP started");
    display.begin();
    debug(INFO, SETUP, "Display connection started");
    display.fillScreen(BACKGROUND_COLOR);
    display.setTextWrap(false);
    display.setRotation(ROTATION);
    debug(INFO, SETUP, "Display initialized");

    //Logo
    display.pushImage(0, 0, 320, 240, testLogo);
    eeprom();
    setupDatabaseConnection();
    mode.setValue(CHART);
    debug(DEBUG, SETUP, "Display SETUP completed");
    debug(DEBUG, SETUP, "");
    delay(1000);
    Meassure::setup();
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
    while(millis()%STAGE_TIME > 0) {}
    handleTouch();
    initDisplay();
    Meassure::loop();
    boolean changed = DisplayV1::getGraphData();
    /*if(mode.getValue() == MENU) {
      Menu::loop();
    } else */
    if(mode.getValue() == MENU) {
      Menu::loop();
    } else if(mode.getValue() == CHART) {
      if(!version.getValue()) {
        DisplayV1::loop(changed);
      } else {
        DisplayV2::loop();
      }
    } else {

    }
    client.loop();
  }

  void Display::initDisplay() {
    if(mode.hasChanged() || (version.hasChanged() && !mode.equals(MENU)) || theme.hasChanged() || language.hasChanged()) {
      if(mode.getValue() == CHART) {
        if(!version.getValue()) {
          DisplayV1::setup();
        } else {
          DisplayV2::setup();
        }
      } else if(mode.getValue() == MENU) {
        Menu::setup();
      } else {

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
      if(p.isTouching()) {
        p.calibrate();
        if(p.isTouching(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_END_X, MENU_ARROW_BACK_START_Y, MENU_ARROW_BACK_END_Y)) {
          if(millis() - lastModeChange >= MENU_BUTTON_COOLDOWN) {
            if(debugSetup.getValue())
              debug(INFO, SETUP, "change Mode");
            if(mode.equals(MENU))
              mode.setValue(CHART);
            else if(mode.equals(CHART))
              mode.setValue(MENU);
            lastModeChange = millis();
          }
        } else if(p.isTouching(MENU_ARROW_RESET_START_X, MENU_ARROW_RESET_END_X, MENU_ARROW_RESET_START_Y, MENU_ARROW_RESET_END_Y)) {
          if(debugSetup.getValue())
            debug(INFO, SETUP, "reset");
          Menu::reset();
        } else if(mode.equals(MENU)) {
          Menu::handleTouch(p);
        }
      }
    } else {
      p = ts.getPoint();
    }
  }
