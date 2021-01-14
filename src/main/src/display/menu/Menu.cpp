/*
 Menu.cpp - Menu-Library for Librarys.
*/

#include "Menu.h"

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
      display.drawLine(0, GRAPH_END_Y, DISPLAY_LENGTH, GRAPH_END_Y, TEXT_COLOR);
      display.pushImage(MENU_ICON_START_X, MENU_ICON_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, menuArrow, WHITE);
      Serial.println("Display drawn");
    }
    drawFirstSection();
    drawSecondSection();
    drawThirdSection();
  }

  extern void Menu::drawFirstSection() {
    if(lastMode != mode && mode == MENU) {
      drawBorder(0, GRAPH_END_Y, DISPLAY_LENGTH, MENU_SECTION_HEIGHT, 1, TEXT_COLOR);
      dPrint("Version 1", MENU_MARGIN_LEFT, (GRAPH_END_Y + MENU_SECTION_HEIGHT*0.5), MENU_SECTION_SIZE, TEXT_COLOR, 3);
    }
    if(version == V1) {
      display.fillCircle(MENU_SLIDER_DOT_END_X, (GRAPH_END_Y + MENU_SLIDER_DOT_HEIGHT), MENU_SLIDER_DOT_RADIUS, BACKGROUND_COLOR);
      display.fillRoundRect(MENU_SLIDER_START_X, MENU_SLIDER1_START_Y, MENU_SLIDER_LENGTH, MENU_SLIDER_HEIGHT, MENU_SLIDER_RADIUS, SLIDER_COLOR_BACKGROUND);
      display.drawRoundRect(MENU_SLIDER_START_X, MENU_SLIDER1_START_Y, MENU_SLIDER_LENGTH, MENU_SLIDER_HEIGHT, MENU_SLIDER_RADIUS, TEXT_COLOR);
      display.fillCircle(MENU_SLIDER_DOT_START_X, (GRAPH_END_Y + MENU_SLIDER_DOT_HEIGHT), MENU_SLIDER_DOT_RADIUS, SLIDER_COLOR_OFF);
      display.drawCircle(MENU_SLIDER_DOT_START_X, (GRAPH_END_Y + MENU_SLIDER_DOT_HEIGHT), MENU_SLIDER_DOT_RADIUS, TEXT_COLOR);
      dPrint("Version 1", MENU_MARGIN_LEFT, (GRAPH_END_Y + MENU_SECTION_HEIGHT*0.5), MENU_SECTION_SIZE, TEXT_COLOR, 3, BACKGROUND_COLOR, "Version 2");
    } else if(version == V2) {
      display.fillCircle(MENU_SLIDER_DOT_START_X, (GRAPH_END_Y + MENU_SLIDER_DOT_HEIGHT), MENU_SLIDER_DOT_RADIUS, BACKGROUND_COLOR);
      display.fillRoundRect(MENU_SLIDER_START_X, MENU_SLIDER1_START_Y, MENU_SLIDER_LENGTH, MENU_SLIDER_HEIGHT, MENU_SLIDER_RADIUS, SLIDER_COLOR_ON_STRIP);
      display.drawRoundRect(MENU_SLIDER_START_X, MENU_SLIDER1_START_Y, MENU_SLIDER_LENGTH, MENU_SLIDER_HEIGHT, MENU_SLIDER_RADIUS, TEXT_COLOR);
      display.fillCircle(MENU_SLIDER_DOT_END_X, (GRAPH_END_Y + MENU_SLIDER_DOT_HEIGHT), MENU_SLIDER_DOT_RADIUS, SLIDER_COLOR_ON);
      display.drawCircle(MENU_SLIDER_DOT_END_X, (GRAPH_END_Y + MENU_SLIDER_DOT_HEIGHT), MENU_SLIDER_DOT_RADIUS, TEXT_COLOR);
      dPrint("Version 2", MENU_MARGIN_LEFT, (GRAPH_END_Y + MENU_SECTION_HEIGHT*0.5), MENU_SECTION_SIZE, TEXT_COLOR, 3, BACKGROUND_COLOR, "Version 1");
    }
  }

  extern void Menu::drawSecondSection() {
    if(lastMode != mode && mode == MENU) {
      drawBorder(0, GRAPH_END_Y+MENU_SECTION_HEIGHT, DISPLAY_LENGTH, MENU_SECTION_HEIGHT, 1, TEXT_COLOR);
      dPrint("Settings", DISPLAY_LENGTH/2, (GRAPH_END_Y + MENU_SECTION_HEIGHT*1.5), MENU_SECTION_SIZE, TEXT_COLOR, 4);
    }
  }

  extern void Menu::drawThirdSection() {
    if(lastMode != mode && mode == MENU) {
      drawBorder(0, GRAPH_END_Y+2*MENU_SECTION_HEIGHT, DISPLAY_LENGTH, MENU_SECTION_HEIGHT, 1, TEXT_COLOR);
      dPrint("Colours", DISPLAY_LENGTH/2, (GRAPH_END_Y + MENU_SECTION_HEIGHT*2.5), MENU_SECTION_SIZE, TEXT_COLOR, 4);
    }
  }
