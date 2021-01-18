/*
  SubMenu.h - SubMenu-Library for Airduino.
*/

#ifndef SubMenu_h
#define SubMenu_h

#include "Arduino.h"
#include "../menuPage/MenuPage.h"

class SubMenu {
  public:
    SubMenu(void);
    SubMenu(char title[15], std::vector<MenuPage> pages, short defaultPage = 0);
    void setup();
    void reset();
    void handleTouch(TSPoint p);
    void clear();

  private:
    void draw();
    void setPage(int page);
    void shiftPage(boolean up);
    boolean checkTouch(TSPoint p);

    short _currentPage;
    short _defaultPage;
    String _title;
    std::vector<MenuPage> _pages;
};

#endif
