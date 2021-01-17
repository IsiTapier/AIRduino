/*
 Menu.cpp - Menu-Library for Librarys.
*/

#include "Menu.h"


  extern int Menu::currentSubMenu = 0;
  extern SubMenu Menu::subMenus[] = {
    {SubMenu("test 1", std::initializer_list<MenuPage> {
        MenuPage(Input(SLIDER, "off", "on", false, true, "value changed"), Input(SLIDER, "Version 1", "Version 2", true, true, "version changed"), Input(EMPTY), Input(SLIDER, "offoffoffoff", "on", false, false, "changed"), Input(EMPTY), Input(SLIDER, "test 1", "test 2", false, true, "Value changed")),
        MenuPage(Input(EMPTY), Input(SLIDER, "off", "on", false, true, "value changed")),
        MenuPage(Input(SLIDER, "off", "on", false, true, "value changed"))
    })},
    {SubMenu("test 2", std::initializer_list<MenuPage> {
        MenuPage(Input(SLIDER, "off", "on", false, true, "value changed")),
        MenuPage(Input(EMPTY), Input(SLIDER, "off", "on")),
        MenuPage(Input(SLIDER))
    })},
    {SubMenu("test 3", std::initializer_list<MenuPage> {
        MenuPage(Input(SLIDER, "off", "on", false, true, "value changed")),
        MenuPage(Input(EMPTY), Input(SLIDER, "off", "on")),
        MenuPage(Input(SLIDER))
    })}
  };

  extern void Menu::setup() {
    Serial.println("Menu SETUP started");
    draw();
    subMenus[currentSubMenu].setup();
    Serial.println("Menu SETUP complete");
    Serial.println();
  }

  extern void Menu::loop() {
    delay(100);
  }

  extern void Menu::draw() {
    if(lastMode != mode && mode == MENU) {
      display.fillScreen(BACKGROUND_COLOR);
      display.fillRect(0, TOP_BAR_HEIGHT, DISPLAY_LENGTH, TOP_BAR_THICKNESS, TEXT_COLOR);
      display.pushImage(MENU_ICON_START_X, MENU_ICON_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, menuArrow, WHITE);
      display.pushImage(MENU_ICON_MARGIN, MENU_ICON_MARGIN, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, resetArrow, BLACK);
      Serial.println("Display drawn");
    }
  }

  extern void Menu::setSubMenu(int subMenu) {
    if(subMenu < 0)
      subMenu = 0;
    if(subMenu >= sizeof(subMenus))
      subMenu = sizeof(subMenus)-1;

    currentSubMenu = subMenu;
    subMenus[currentSubMenu].setup();
  }

  extern void Menu::shiftSubMenu(boolean left) {
    if(left) {
      if(currentSubMenu <= 0)
        setSubMenu(sizeof(subMenus)-1);
      else
        setSubMenu(currentSubMenu-1);
    } else {
      if(currentSubMenu >= sizeof(subMenus)-1)
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
    if(false) {
      shiftSubMenu(true);
      return(true);
    } else if(false) {
      shiftSubMenu(false);
      return(true);
    } else
      return(false);
  }

  extern void Menu::reset() {
    if(currentSubMenu != DEFAULT_SUB_MENU)
      setSubMenu(DEFAULT_SUB_MENU);
    else
      subMenus[currentSubMenu].reset();
  }
