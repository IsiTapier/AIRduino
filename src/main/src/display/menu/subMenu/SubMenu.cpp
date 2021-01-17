/*
 SubMenu.cpp - SubMenu-Library for Librarys.
*/

#include "SubMenu.h"

SubMenu::SubMenu(void) {}
SubMenu::SubMenu(String title, std::vector<MenuPage> pages, int defaultPage) {
  _title = title;
  _pages = pages;
  _currentPage = defaultPage;
  _defaultPage = defaultPage;
}

void SubMenu::setup() {
  draw();
  _pages.at(_currentPage).setup();
}

void SubMenu::draw() {
  dPrint(_title, DISPLAY_LENGTH/2, STATUS_MARGIN_TOP, MENU_TITLE_SIZE, TEXT_COLOR, 1); //TODO Title size
   //TODO side
  Serial.println("Page "+String(_currentPage)+" drawn");
}

void SubMenu::setPage(int page) {
  if(page < 0)
    page = 0;
  if(page >= _pages.size())
    page = _pages.size()-1;

  _currentPage = page;
  _pages.at(_currentPage).setup();
}

void SubMenu::shiftPage(boolean left) {
  if(left) {
    if(_currentPage <= 0)
      setPage(_pages.size()-1);
    else
      setPage(_currentPage-1);
  } else {
    if(_currentPage >= _pages.size()-1)
      setPage(0);
    else
      setPage(_currentPage+1);
  }
}

void SubMenu::handleTouch(TSPoint p) {
  if(!checkTouch(p)) {
    _pages.at(_currentPage).handleTouch(p);
  }
}

boolean SubMenu::checkTouch(TSPoint p) {
  if(false) {
    shiftPage(true);
    return(true);
  } else if(false) {
    shiftPage(false);
    return(true);
  } else
    return(false);
}

void SubMenu::reset() {
  if(_currentPage != _defaultPage)
    setPage(_defaultPage);
  else
    _pages.at(_currentPage).reset();
}
