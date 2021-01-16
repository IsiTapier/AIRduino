/*
  SubMenu.h - SubMenu-Library for Airduino.
*/

#ifndef SubMenu_h
#define SubMenu_h

#include "Arduino.h"
#include "../menuPage/MenuPage.h"

class SubMenu {
  public:
    SubMenu(String title, std::initializer_list<MenuPage> pages, int currentPage, int defaultPage = 1);
    void init();
    void drawTop();
    void drawSide();
    void handleTouch(TSPoint p);

  private:
    int _currentPage;
    int _defaultPage;
    String _title;
    std::initializer_list<MenuPage> _pages;
};

#endif
