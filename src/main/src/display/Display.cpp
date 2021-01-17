/*
 Display.cpp - Display-Library for Librarys.
*/

#include "Display.h"

  extern TSPoint Display::p;

  extern void Display::setup() {
    Serial.println("DISPLAY SETUP started");
    display.begin();
    Serial.println("Display connection started");
    display.fillScreen(BACKGROUND_COLOR);
    display.setTextWrap(false);
    display.setRotation(ROTATION);
    Serial.println("Display initialized");
    Serial.println("Touch calibration started");
    dPrint("Calibration", round(DISPLAY_LENGTH/2), round(DISPLAY_HEIGHT/2), TOUCH_CALIBRATION_HEADER_SIZE, TEXT_COLOR, 7);
    dPrint("press blue box", round(DISPLAY_LENGTH/2), round(DISPLAY_HEIGHT/2), TOUCH_CALIBRATION_TEXT_SIZE, TEXT_COLOR, 1);
    ts.calibration();
    Serial.println("Touch calibrated");
    loadingScreen();
    lastMode = mode;
    mode = CHART;
    Serial.println("DISPLAY SETUP complete");
    Serial.println();
    Meassure::setup();
  }

  extern void Display::loop() {
    handleTouch();
    initDisplay();
    if(mode == MENU) {
      Menu::loop();
    } else if(mode == CHART) {
      Meassure::loop();
      handleTouch();
      if(version == V1) {
        DisplayV1::loop();
      } else if(version == V2) {
        DisplayV2::loop();
      } else {

      }
    } else {

    }
  }

  extern void Display::initDisplay() {
    if(lastMode != mode || lastVersion != version) {
      if(mode == CHART) {
        if(version == V1) {
          DisplayV1::setup();
        } else if(version == V2) {
          DisplayV2::setup();
        } else {

        }
      } else if(mode == MENU) {
        Menu::setup();
      } else {

      }
    }
    lastMode = mode;
    lastVersion = version;
  }

  extern void Display::handleTouch() {
    if(!p.isTouching()) {
      // a point object holds x y and z coordinates
      p = ts.getPoint();
      // we have some minimum pressure we consider 'valid'
      // pressure of 0 means no pressing!
      if(p.isTouching()) {
        p.calibrate();
        p.print();
        if(p.isTouching(MENU_ICON_START_X, MENU_ICON_END_X, MENU_ICON_START_Y, MENU_ICON_END_Y)) {
          Serial.println("change Mode");
          if(mode == MENU)
            mode = (Mode) CHART;
          else if(mode == CHART)
            mode = (Mode) MENU;
        } else if(p.isTouching(MENU_ICON_MARGIN, (MENU_ICON_MARGIN + MENU_ICON_LENGTH), MENU_ICON_MARGIN, (MENU_ICON_MARGIN + MENU_ICON_HEIGHT))) {
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
