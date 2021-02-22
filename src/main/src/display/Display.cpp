/*
 Display.cpp - Display-Library for Librarys.
*/

#include "Display.h"

  extern TSPoint Display::p;

  extern void Display::setup() {
    debug(DEBUG, SETUP, "Display SETUP started");
    setupDatabaseConnection();
    display.begin();
    debug(INFO, SETUP, "Display connection started");
    display.fillScreen(BACKGROUND_COLOR);
    display.setTextWrap(false);
    display.setRotation(ROTATION);
    debug(INFO, SETUP, "Display initialized");
    eeprom();
    loadingScreen();
    lastMode = mode;
    mode = CHART;
    debug(DEBUG, SETUP, "Display SETUP completed");
    debug(DEBUG, SETUP, "");
    Meassure::setup();
  }

  extern void Display::eeprom() {
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
      //ts.calibration();
    if(general::debugSetup.getValue() && general::debugPriority.getValue()) {
      debug(INFO, SETUP, "EEPROM: sensor", EEPROM.read(0));
      debug(INFO, SETUP, "EEPROM: xmin", EEPROM.readShort(XMIN));
      debug(INFO, SETUP, "EEPROM: xmax", EEPROM.readShort(XMAX));
      debug(INFO, SETUP, "EEPROM: ymin", EEPROM.readShort(YMIN));
      debug(INFO, SETUP, "EEPORM: ymax", EEPROM.readShort(YMAX));
    }
  }

  extern void Display::loop() {
    handleTouch();
    initDisplay();
    if(mode == MENU) {
      Menu::loop();
    } else if(mode == CHART) {
      Meassure::loop();
      handleTouch();
      if(!general::version.getValue()) {
        DisplayV1::loop();
      } else {
        DisplayV2::loop();
      }
    } else {

    }
    for(short x = 0; x <= 100; x++) {
        client.loop();
    }

  }

  extern void Display::initDisplay() {
    if(lastMode != mode || lastVersion != general::version.getValue() || general::theme.getValue() != general::theme.getOldValue()) {
      if(mode == CHART) {
        if(!general::version.getValue()) {
          DisplayV1::setup();
        } else {
          DisplayV2::setup();
        }
      } else if(mode == MENU && lastMode != MENU || general::theme.getValue() != general::theme.getOldValue()) {
        Menu::setup();
      } else {

      }
    }
    lastMode = mode;
    lastVersion = (Version) general::version.getValue();
    general::theme.setValue(general::theme.getValue());
  }

  extern void Display::handleTouch() {
    if(!p.isTouching()) {
      // a point object holds x y and z coordinates
      p = ts.getPoint();
      // we have some minimum pressure we consider 'valid'
      // pressure of 0 means no pressing!
      //template:: p.isTouching(PLACEHOLDER_PLACE_START_X, PLACEHOLDER_PLACE_END_X, PLACEHOLDER_PLACE_START_Y, PLACEHOLDER_PLACE_END_Y)
      if(p.isTouching()) {
        p.calibrate();
        if(p.isTouching(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_END_X, MENU_ARROW_BACK_START_Y, MENU_ARROW_BACK_END_Y)) {
          if(general::debugSetup.getValue() && general::debugPriority.getValue())
            debug(INFO, SETUP, "change Mode");
          if(mode == MENU)
            mode = (Mode) CHART;
          else if(mode == CHART)
            mode = (Mode) MENU;
        } else if(p.isTouching(MENU_ARROW_RESET_START_X, MENU_ARROW_RESET_END_X, MENU_ARROW_RESET_START_Y, MENU_ARROW_RESET_END_Y)) {
          if(general::debugSetup.getValue() && general::debugPriority.getValue())
            debug(INFO, SETUP, "reset");
          Menu::reset();
        } else if(mode == MENU) {
          Menu::handleTouch(p);
        }
      }
    } else {
      p = ts.getPoint();
    }
  }
