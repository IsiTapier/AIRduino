/*
 Menu.cpp - Menu-Library for Librarys.
*/

#include "Menu.h"


  short reportMenuOpen = 0;
  int Menu::currentSubMenu = 0;
  boolean test = false;

  SubMenu subMenus[] = {
    {SubMenu("General", {
        MenuPage(
          //Input(&general::version),
          Input(&general::version),
          Input(&general::theme),
          Input(&general::reportSystem),
          Input(&general::language),
          Input(&general::sound),
          Input(&general::autoTimeReset)         
        ),      
        MenuPage(
          //Input(&general::developperSettings),
          Input(&developper::deviceInfo),
          Input(&general::blink),
          Input(&general::graph_speed),
          Input(&general::ventilating_timeout)
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
          Input(&colorModes::c_slider),
          Input(&colorModes::showTopBar)
        )
    })},
    {SubMenu("Developer", {
        MenuPage(
          Input(&developper::calibrateMin),
          Input(&general::empty),
          Input(&developper::deviceInfo),
          Input(&developper::calibrateMax),
          Input(&general::calibrateTouch)
        ),
        MenuPage(
          Input(&general::debugPriority),
          Input(&general::debugSetup),
          Input(&general::debugSensor),
          Input(&general::debugDisplay),
          Input(&general::debugMenu),
          Input(&general::debugTouch)
        ),
        MenuPage(
          Input(&general::data),
          Input(&general::blink),//Input(&general::kernMode),
          Input(&general::graph_speed),
          Input(&general::segments),
          Input(&general::blink_thickness),
          Input(&general::ventilating_timeout)
        ),
        MenuPage(
          Input(&general::debugDatabase)
        )
    }, 0, general::developperSettings.getValuePointer())},
    {SubMenu(REPORT_MENU_TITLE, {
        MenuPage(
          Input(&report::sensorError),
          Input(&report::sensorWrong),
          Input(&report::graphicalBug),
          Input(&report::acousticBug),
          Input(&report::helpShortterm),
          Input(&report::developerEmail)
        )
    }, 0, &reportMenuOpen)}
  };

  void Menu::setup() {
    debug(DEBUG, SETUP, "Menu SETUP started");
    if(mode.hasChanged() || theme.hasChanged())
      draw();
    if(mode.hasChanged() && reportMenuOpen)
      setSubMenu(DEFAULT_SUB_MENU);
    subMenus[currentSubMenu].setup(true);
    debug(DEBUG, SETUP, "Menu SETUP completed");
    //request decision cooldown
    if(mode.equaled(LOADINGSCREEN))
      delay(REQUESTDECISIONCOOLDOWN);
  }

  void Menu::loop() {
  }

  void Menu::draw() {
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

  boolean Menu::setSubMenu(int subMenu, boolean allowHidden) {
    subMenus[currentSubMenu].clear();
    if(subMenu < 0)
      subMenu = 0;
    if(subMenu >= sizeOf(subMenus))
      subMenu = sizeOf(subMenus)-1;
    if(currentSubMenu == getSubMenu(REPORT_MENU_TITLE) && reportMenuOpen) {
      currentSubMenu = DEFAULT_SUB_MENU;
      reportMenuOpen = false;
    } else
      currentSubMenu = subMenu;
    if(subMenus[currentSubMenu].isHidden() && !allowHidden)
      return false;
    subMenus[currentSubMenu].setup();
    return true;
  }

  void Menu::shiftSubMenu(boolean left) {
    if(left) {
      if(currentSubMenu <= 0) {
        if(setSubMenu(sizeOf(subMenus)-1))
          return;
      } else {
        if(setSubMenu(currentSubMenu-1))
          return;
      }
    } else {
      if(currentSubMenu >= sizeOf(subMenus)-1) {
        if(setSubMenu(0))
          return;
      } else {
        if(setSubMenu(currentSubMenu+1))
          return;
      }
    }
    shiftSubMenu(left);
  }

  void Menu::handleTouch(TSPoint p) {
    if(!checkTouch(p)) {
      subMenus[currentSubMenu].handleTouch(p);
    }
  }

  boolean Menu::checkTouch(TSPoint p) {
    if(p.isTouching(MENU_ARROW_LEFT_START_X-3, MENU_ARROW_LEFT_END_X+3, MENU_ARROW_LEFT_START_Y-3, MENU_ARROW_LEFT_END_Y+3)) {
      shiftSubMenu(true);
      return(true);
    } else if(p.isTouching(MENU_ARROW_RIGHT_START_X-3, MENU_ARROW_RIGHT_END_X+3, MENU_ARROW_RIGHT_START_Y-3, MENU_ARROW_RIGHT_END_Y+3)) {
      shiftSubMenu(false);
      return(true);
    } else
      return(false);
  }

  void Menu::reset() {
    /*if(currentSubMenu != DEFAULT_SUB_MENU)
      setSubMenu(DEFAULT_SUB_MENU);
    else*/
    draw();
    for(int i = 0; i < sizeOf(subMenus); i++)
      subMenus[i].reset(general::mode.equals(MENU) ? i == currentSubMenu ? true : false : false);
  }

  void Menu::openReportMenu() {
    reportMenuOpen = true;
    setSubMenu(getSubMenu(REPORT_MENU_TITLE));
  }

  int Menu::getSubMenu(String title) {
    for(int i = 0; i < sizeOf(subMenus); i++) {
      if(subMenus[i].getTitle().equals(title))
        return(i);
    }
    return DEFAULT_SUB_MENU;
  }
