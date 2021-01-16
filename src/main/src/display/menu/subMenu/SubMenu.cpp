/*
 SubMenu.cpp - SubMenu-Library for Librarys.
*/

#include "SubMenu.h"

extern int _currentPage;
extern int _defaultPage;
extern String _title;
extern std::initializer_list<MenuPage> _pages;

extern SubMenu::SubMenu(String title, std::initializer_list<MenuPage> pages, int currentPage, int defaultPage) {
  _title = title;
  _pages = pages;
  _currentPage = currentPage;
  _defaultPage = defaultPage;
}

extern void SubMenu::init() {
  drawTop();
  drawSide();
  _(_pages)[_currentPage].init();
}

extern void SubMenu::drawTop() {

}

extern void SubMenu::drawSide() {

}

extern void SubMenu::handleTouch(TSPoint p) {
  if(!checkTouch(p)) {
    _(_pages)[_currentPage].handleTouch(p);
  }
}
