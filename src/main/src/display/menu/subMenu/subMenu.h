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
    SubMenu(String title, std::vector<MenuPage> pages, int defaultPage = 0);
    void setup();
    void reset();
    void handleTouch(TSPoint p);

  private:
    void draw();
    void setPage(int page);
    void shiftPage(boolean left);
    boolean checkTouch(TSPoint p);

    int _currentPage;
    int _defaultPage;
    String _title;
    std::vector<MenuPage> _pages;
};

#endif
