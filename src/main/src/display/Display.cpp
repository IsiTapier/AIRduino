/*
 Display.cpp - Display-Library for Librarys.
*/

#include "Display.h"

  extern TSPoint Display::p;

  extern void Display::setup() {
    Serial.println("DISPLAY SETUP started");
    yield();
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
      if(!general::version.getValue()) {
        DisplayV1::loop();
      } else {
        DisplayV2::loop();
      }
    } else {

    }
  }

  extern void Display::initDisplay() {
    if(lastMode != mode || lastVersion != general::version.getValue()) {
      if(mode == CHART) {
        if(!general::version.getValue()) {
          DisplayV1::setup();
        } else {
          DisplayV2::setup();
        }
      } else if(mode == MENU) {
        Menu::setup();
      } else {

      }
    }
    lastMode = mode;
    lastVersion = (Version) general::version.getValue();
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
        p.print();
        if(p.isTouching(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_END_X, MENU_ARROW_BACK_START_Y, MENU_ARROW_BACK_END_Y)) {
          Serial.println("change Mode");
          if(mode == MENU)
            mode = (Mode) CHART;
          else if(mode == CHART)
            mode = (Mode) MENU;
        } else if(p.isTouching(MENU_ARROW_RESET_START_X, MENU_ARROW_RESET_END_X, MENU_ARROW_RESET_START_Y, MENU_ARROW_RESET_END_Y)) {
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
