/*
  SubMenu.h - SubMenu-Library for Airduino.
*/

#ifndef SubMenu_h
#define SubMenu_h

#include "Arduino.h"
#include "../menuPage/MenuPage.h"

class SubMenu {
  public:
    //template <class MenuPage>
    SubMenu(String title, int numberPages, int currentPage, int defaultPage = 1);
    void init();
    void handleTouch(TSPoint p);

  private:
    boolean checkTouch(TSPoint p);

    int _pages;
    int _currentPage;
    int _defaultPage;
    String _title;
    MenuPage pages[];
};

#endif
