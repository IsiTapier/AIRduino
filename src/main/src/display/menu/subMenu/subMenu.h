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
    SubMenu(const char* title, std::vector<MenuPage> pages, short defaultPage = 0, void* hide = 0);
    void setup(boolean redraw = false);
    void reset(boolean active = false);
    void handleTouch(TSPoint p);
    void clear();
    boolean isHidden();
    String getTitle();

  private:
    void draw();
    void setPage(int page);
    void shiftPage(boolean up);
    boolean checkTouch(TSPoint p);

    short _currentPage;
    short _defaultPage;
    String _title;
    std::vector<MenuPage> _pages;
    boolean* _hide;
};

#endif
