/*
 Display.cpp - Display-Library for Librarys.
*/

#include "Display.h"

  extern TSPoint Display::p;

  extern void Display::setup() {
    if(general::debugSetup.getValue() && general::debug.getValue())
      Serial.println("DISPLAY SETUP started");
    display.begin();
    if(general::debugSetup.getValue() && general::debug.getValue())
      Serial.println("Display connection started");
    display.fillScreen(BACKGROUND_COLOR);
    display.setTextWrap(false);
    display.setRotation(ROTATION);
    if(general::debugSetup.getValue() && general::debug.getValue())
      Serial.println("Display initialized");
    eeprom();
    loadingScreen();
    lastMode = mode;
    mode = CHART;
    if(general::debugSetup.getValue() && general::debug.getValue())
      Serial.println("DISPLAY SETUP complete");
    Serial.println();
    Meassure::setup();
  }

  extern void Display::eeprom() {
    EEPROM.begin(EEPROM_SIZE);
    //   RESET EEPROM   //
    EEPROM.write(0, 1);/*
    EEPROM.writeShort(XMIN, 0);
    EEPROM.writeShort(XMAX, 0);
    EEPROM.writeShort(YMIN, 0);
    EEPROM.writeShort(YMAX, 0);*/
    EEPROM.commit();
    if(EEPROM.read(0) == 255) {
      EEPROM.write(0, 0);
      EEPROM.commit();
    }
    if(EEPROM.readShort(XMIN) == EEPROM.readShort(XMAX) || EEPROM.readShort(YMIN) == EEPROM.readShort(YMAX))
    //  ts.calibration();
    if(general::debugSetup.getValue() && general::debug.getValue()) {
      Serial.println("EEPROM: sensor: "+String(EEPROM.read(0)));
      Serial.println("EEPROM: xmin: "+String(EEPROM.readShort(XMIN)));
      Serial.println("EEPROM: xmax: "+String(EEPROM.readShort(XMAX)));
      Serial.println("EEPROM: ymin: "+String(EEPROM.readShort(YMIN)));
      Serial.println("EEPORM: ymax: "+String(EEPROM.readShort(YMAX)));
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
  }

  extern void Display::initDisplay() {
    if(lastMode != mode || lastVersion != general::version.getValue() || general::darkMode.getValue() != general::darkMode.getOldValue()) {
      if(mode == CHART) {
        if(!general::version.getValue()) {
          DisplayV1::setup();
        } else {
          DisplayV2::setup();
        }
      } else if(mode == MENU && lastMode != MENU || general::darkMode.getValue() != general::darkMode.getOldValue()) {
        Menu::setup();
      } else {

      }
    }
    lastMode = mode;
    lastVersion = (Version) general::version.getValue();
    general::darkMode.setValue(general::darkMode.getValue());
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
          if(general::debugSetup.getValue() && general::debug.getValue())
            Serial.println("change Mode");
          if(mode == MENU)
            mode = (Mode) CHART;
          else if(mode == CHART)
            mode = (Mode) MENU;
        } else if(p.isTouching(MENU_ARROW_RESET_START_X, MENU_ARROW_RESET_END_X, MENU_ARROW_RESET_START_Y, MENU_ARROW_RESET_END_Y)) {
          if(general::debugSetup.getValue() && general::debug.getValue())
            Serial.println("reset");
          Menu::reset();
        } else if(mode == MENU) {
          Menu::handleTouch(p);
        }
      }
    } else {
      p = ts.getPoint();
    }
  }
