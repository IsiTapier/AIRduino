/*
 SubMenu.cpp - SubMenu-Library for Librarys.
*/

#include "SubMenu.h"

extern int _pages;
extern int _currentPage;
extern int _defaultPage;
extern String _title;
extern MenuPage pages[];

extern SubMenu::SubMenu(String title,  int numberPages, int currentPage, int defaultPage) {

}

extern void SubMenu::init() {

}

extern void SubMenu::handleTouch(TSPoint p) {
  if(!checkTouch(p)) {
    pages[_currentPage].checkTouch(p);
  }
}

extern boolean SubMenu::checkTouch(TSPoint p) {

}
