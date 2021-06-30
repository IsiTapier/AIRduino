/*
  Menu.h - Menu-Library for Airduino.
*/

#ifndef Menu_h
#define Menu_h

#include "Arduino.h"
#include "../../util/Util.h"
#include "subMenu/SubMenu.h"
#include "../../util/settings/Settings.h"

extern SubMenu subMenus[];

//temporary
#define DEFAULT_SUB_MENU 0
#define REPORT_MENU_TITLE "Hilfe"
#define LINE_COLOR (BLACK_MODE) ? GREY : GREY

class Menu {
  public:
    static void setup();
    static void loop();
    static void reset();
    static void handleTouch(TSPoint p);
    static void openReportMenu();

  private:
    static void draw();
    static boolean setSubMenu(int subMenu, boolean allowHidden = false);
    static void shiftSubMenu(boolean left);
    static boolean checkTouch(TSPoint p);
    static int getSubMenu(String title);

    static int currentSubMenu;

};

#endif
