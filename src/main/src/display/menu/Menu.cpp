/*
 Menu.cpp - Menu-Library for Librarys.
*/

#include "Menu.h"

  extern int Menu::currentSubMenu = 0;
  SubMenu subMenus[] = {
    {SubMenu("General", {
        MenuPage(
          Input(&general::version),
          Input(&general::theme),
          Input(&general::sound),
          Input(&general::blink),
          Input(&general::data),
          Input(&general::calibrateTouch)
        ),
        MenuPage(
          Input(&general::language),
          Input(&general::kernMode),
          Input(&general::graph_speed),
          Input(&general::segments),
          Input(&general::blink_thickness),
          Input(&general::ventilating_timeout)
        ),
        MenuPage(
          Input(&general::debugPriority),
          Input(&general::debugSetup),
          Input(&general::debugSensor),
          Input(&general::debugDisplay),
          Input(&general::debugMenu),
          Input(&general::debugTouch)
        )
    })},
    {SubMenu("Farb-Modi", {
        MenuPage(
          Input(&colorModes::c_design),
          Input(&colorModes::c_chart),
          Input(&colorModes::c_bar),
          Input(&colorModes::c_state),
          Input(&colorModes::c_time),
          Input(&colorModes::c_value)
        ),
        MenuPage(
          Input(&colorModes::c_slider)
        )
    })},
    {SubMenu("Farben", {
        MenuPage(

        ),
        MenuPage(

        ),
        MenuPage(

        )
    })}
  };

  void Menu::setup() {
    debug(DEBUG, SETUP, "Menu SETUP started");
    draw();
    subMenus[currentSubMenu].setup();
    debug(DEBUG, SETUP, "Menu SETUP completed");
  }

  void Menu::loop() {
    delay(100);
  }

  void Menu::draw() {
    if(general::mode.hasChanged() && general::mode.equals(MENU) || general::theme.hasChanged()) {
      //draw menu top
      display.fillScreen(BACKGROUND_COLOR);
      display.fillRect(0, TOP_BAR_HEIGHT, DISPLAY_LENGTH, TOP_BAR_THICKNESS, TEXT_COLOR);
      display.fillRect(MENU_SIDE_X, MENU_SIDE_Y, MENU_SIDE_THICKNESS, MENU_SIDE_HEIGHT, TEXT_COLOR);
      display.fillRect(MENU_SIDE_BAR_X, MENU_SIDE_BAR_Y, MENU_SIDE_TEXT_LENGTH, MENU_SIDE_BAR_THICKNESS, TEXT_COLOR);
      display.pushImage(MENU_ARROW_LEFT_START_X, MENU_ARROW_LEFT_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, arrowLeft, WHITE);
      display.pushImage(MENU_ARROW_RIGHT_START_X, MENU_ARROW_RIGHT_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, arrowRight, WHITE);
      display.pushImage(MENU_ARROW_UP_START_X, MENU_ARROW_UP_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, arrowUp, WHITE);
      display.pushImage(MENU_ARROW_DOWN_START_X, MENU_ARROW_DOWN_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, arrowDown, WHITE);
      if(general::theme.getValue()) {
        display.pushImage(MENU_ARROW_RESET_START_X, MENU_ARROW_RESET_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, arrowResetDark, BLACK);
        display.pushImage(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, homeDark, BLACK);
      } else {
        display.pushImage(MENU_ARROW_RESET_START_X, MENU_ARROW_RESET_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, arrowResetLight, WHITE);
        display.pushImage(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, homeLight, WHITE);
      }
      //draw menu bottom
      debug(INFO, SETUP, "Display drawn");
    }
  }

  void Menu::setSubMenu(int subMenu) {
    if(subMenu < 0)
      subMenu = 0;
    if(subMenu >= sizeOf(subMenus))
      subMenu = sizeOf(subMenus)-1;
    subMenus[currentSubMenu].clear();
    currentSubMenu = subMenu;
    subMenus[currentSubMenu].setup();
  }

  void Menu::shiftSubMenu(boolean left) {
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

  void Menu::handleTouch(TSPoint p) {
    if(!checkTouch(p)) {
      subMenus[currentSubMenu].handleTouch(p);
    }
  }

  boolean Menu::checkTouch(TSPoint p) {
    if(p.isTouching(MENU_ARROW_LEFT_START_X, MENU_ARROW_LEFT_END_X, MENU_ARROW_LEFT_START_Y, MENU_ARROW_LEFT_END_Y)) {
      shiftSubMenu(true);
      return(true);
    } else if(p.isTouching(MENU_ARROW_RIGHT_START_X, MENU_ARROW_RIGHT_END_X, MENU_ARROW_RIGHT_START_Y, MENU_ARROW_RIGHT_END_Y)) {
      shiftSubMenu(false);
      return(true);
    } else
      return(false);
  }

  void Menu::reset() {          //TODO reset
    /*if(currentSubMenu != DEFAULT_SUB_MENU)
      setSubMenu(DEFAULT_SUB_MENU);
    else*/
      for(int i = 0; i < sizeOf(subMenus); i++)
        subMenus[i].reset();
  }
