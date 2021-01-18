/*
 Menu.cpp - Menu-Library for Librarys.
*/

#include "Menu.h"

using namespace test;

  extern int Menu::currentSubMenu = 0;
  SubMenu subMenus[] = {
    {SubMenu("test 1", {


        MenuPage(
          Input(test19),
          Input(test20),
          Input(test21))
    })}
  };

  extern void Menu::setup() {
    Serial.println("Menu SETUP started");
    draw();
    Serial.println(currentSubMenu);
    subMenus[currentSubMenu].setup();
    Serial.println("Menu SETUP complete");
    Serial.println();
  }

  extern void Menu::loop() {
    delay(100);
  }

  extern void Menu::draw() {
    if(lastMode != mode && mode == MENU) {
      //draw menu top
      display.fillScreen(BACKGROUND_COLOR);
      display.fillRect(0, TOP_BAR_HEIGHT, DISPLAY_LENGTH, TOP_BAR_THICKNESS, TEXT_COLOR);
      display.fillRect(MENU_SIDE_X, MENU_SIDE_Y, MENU_SIDE_THICKNESS, MENU_SIDE_HEIGHT, TEXT_COLOR);
      display.fillRect(MENU_SIDE_BAR_X, MENU_SIDE_BAR_Y, MENU_SIDE_TEXT_LENGTH, MENU_SIDE_BAR_THICKNESS, TEXT_COLOR);
      display.pushImage(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, home, BLACK);
      display.pushImage(MENU_ARROW_RESET_START_X, MENU_ARROW_RESET_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, arrowReset, BLACK);
      display.pushImage(MENU_ARROW_LEFT_START_X, MENU_ARROW_LEFT_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, arrowLeft, WHITE);
      display.pushImage(MENU_ARROW_RIGHT_START_X, MENU_ARROW_RIGHT_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, arrowRight, WHITE);
      display.pushImage(MENU_ARROW_UP_START_X, MENU_ARROW_UP_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, arrowUp, WHITE);
      display.pushImage(MENU_ARROW_DOWN_START_X, MENU_ARROW_DOWN_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, arrowDown, WHITE);
      //draw menu bottom
      Serial.println("Display drawn");
    }
  }

  extern void Menu::setSubMenu(int subMenu) {
    if(subMenu < 0)
      subMenu = 0;
    if(subMenu >= sizeOf(subMenus))
      subMenu = sizeOf(subMenus)-1;
    subMenus[currentSubMenu].clear();
    currentSubMenu = subMenu;
    subMenus[currentSubMenu].setup();
  }

  extern void Menu::shiftSubMenu(boolean left) {
    if(left) {
      if(currentSubMenu <= 0)
        setSubMenu(sizeOf(subMenus)-1);
      else
        setSubMenu(currentSubMenu-1);
    } else {
      if(currentSubMenu >= sizeOf(subMenus)-1)
        setSubMenu(0);
      else
        setSubMenu(currentSubMenu+1);
    }
  }

  extern void Menu::handleTouch(TSPoint p) {
    if(!checkTouch(p)) {
      subMenus[currentSubMenu].handleTouch(p);
    }
  }

  extern boolean Menu::checkTouch(TSPoint p) {
    if(p.isTouching(MENU_ARROW_LEFT_START_X, MENU_ARROW_LEFT_END_X, MENU_ARROW_LEFT_START_Y, MENU_ARROW_LEFT_END_Y)) {
      shiftSubMenu(true);
      return(true);
    } else if(p.isTouching(MENU_ARROW_RIGHT_START_X, MENU_ARROW_RIGHT_END_X, MENU_ARROW_RIGHT_START_Y, MENU_ARROW_RIGHT_END_Y)) {
      shiftSubMenu(false);
      return(true);
    } else
      return(false);
  }

  extern void Menu::reset() {          //TODO reset
    /*if(currentSubMenu != DEFAULT_SUB_MENU)
      setSubMenu(DEFAULT_SUB_MENU);
    else*/
      for(int i = 0; i < sizeOf(subMenus); i++)
        subMenus[i].reset();
  }
