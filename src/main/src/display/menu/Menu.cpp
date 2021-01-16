/*
 Menu.cpp - Menu-Library for Librarys.
*/

#include "Menu.h"

  extern Slider Menu::firstSection(0, "Version 1", "Version 2", false, true, "Version changed");
  extern Slider Menu::secondSection(1, "test1", "test2");
  extern Slider Menu::thirdSection(2, "test1", "test2");
  extern Slider Menu::fourthSection(3, "test1", "test2");
  extern Slider Menu::fifthSection(4, "test1", "test2");
  extern Slider Menu::sixthSection(5, "test1", "test2");

  extern void Menu::setup() {
    Serial.println("Menu SETUP started");
    drawDisplay();
    Serial.println("Menu SETUP complete");
    Serial.println();
  }

  extern void Menu::loop() {
    delay(100);
  }

  extern void Menu::drawDisplay() {
    if(lastMode != mode && mode == MENU) {
      display.fillScreen(BACKGROUND_COLOR);
      dPrint("Menu", DISPLAY_LENGTH/2, STATUS_MARGIN_TOP, MENU_TITLE_SIZE, TEXT_COLOR, 1);
      display.fillRect(0, TOP_BAR_HEIGHT, DISPLAY_LENGTH, TOP_BAR_THICKNESS, TEXT_COLOR);
      display.pushImage(MENU_ICON_START_X, MENU_ICON_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, menuArrow, WHITE);
      display.pushImage(MENU_ICON_MARGIN, MENU_ICON_MARGIN, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, resetArrow, BLACK);
      Serial.println("Display drawn");
      firstSection.init();
      secondSection.init();
      thirdSection.init();
      fourthSection.init();
      fifthSection.init();
      sixthSection.init();
    }
  }

  extern void Menu::checkTouch(TSPoint p) {
    firstSection.checkTouch(p);
    secondSection.checkTouch(p);
    thirdSection.checkTouch(p);
    fourthSection.checkTouch(p);
    fifthSection.checkTouch(p);
    sixthSection.checkTouch(p);
    version = firstSection.getValue() ? V2 : V1;
  }

  extern void Menu::reset() {
    firstSection.setValue(false);
    secondSection.setValue(false);
    thirdSection.setValue(false);
    fourthSection.setValue(false);
    fifthSection.setValue(false);
    sixthSection.setValue(false);
  }
