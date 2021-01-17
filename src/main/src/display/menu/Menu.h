/*
  Menu.h - Menu-Library for Airduino.
*/

#ifndef Menu_h
#define Menu_h

#include "Arduino.h"
#include "../../util/Util.h"
#include "subMenu/SubMenu.h"

//temporary
#define DEFAULT_SUB_MENU 0

class Menu {

  public:
    static void setup();
    static void loop();
    static void reset();
    static void handleTouch(TSPoint p);

  private:
    static void draw();
    static void setSubMenu(int subMenu);
    static void shiftSubMenu(boolean left);
    static boolean checkTouch(TSPoint p);

    static SubMenu subMenus[];
    static int currentSubMenu;

};

#endif
