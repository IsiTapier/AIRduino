/*
 SubMenu.cpp - SubMenu-Library for Librarys.
*/

#include "SubMenu.h"

SubMenu::SubMenu(void) {}
SubMenu::SubMenu(char title[15], std::vector<MenuPage> pages, short defaultPage) {
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
  dPrint(_currentPage+1, MENU_SIDE_CENTER_X, MENU_SIDE_CENTER_Y, MENU_SIDE_SIZE, TEXT_COLOR, 7);
  dPrint(_pages.size(), (MENU_SIDE_CENTER_X + MENU_SIDE_BAR_THICKNESS + MENU_SIDE_SIZE), MENU_SIDE_CENTER_Y, MENU_SIDE_SIZE, TEXT_COLOR, 1);
  Serial.println("Submenu "+_title+" drawn");
}

void SubMenu::clear() {
  dPrint(_title, DISPLAY_LENGTH/2, STATUS_MARGIN_TOP, MENU_TITLE_SIZE, BACKGROUND_COLOR, 1); //TODO Title size
  dPrint(_currentPage+1, MENU_SIDE_CENTER_X, MENU_SIDE_CENTER_Y, MENU_SIDE_SIZE, BACKGROUND_COLOR, 7);
  dPrint(_pages.size(), MENU_SIDE_CENTER_X, MENU_SIDE_CENTER_Y, MENU_SIDE_SIZE, BACKGROUND_COLOR, 1);
  Serial.println("Submenu "+_title+" cleared");
}

void SubMenu::setPage(int page) {
  if(page < 0)
    page = 0;
  if(page >= _pages.size())
    page = _pages.size()-1;
  dPrint(_currentPage+1, MENU_SIDE_CENTER_X, MENU_SIDE_CENTER_Y, MENU_SIDE_SIZE, BACKGROUND_COLOR, 7);
  _currentPage = page;
  _pages.at(_currentPage).setup();
  dPrint(_currentPage+1, MENU_SIDE_CENTER_X, MENU_SIDE_CENTER_Y, MENU_SIDE_SIZE, TEXT_COLOR, 7);
}

void SubMenu::shiftPage(boolean up) {
  if(up) {
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
  if(p.isTouching(MENU_ARROW_UP_START_X, MENU_ARROW_UP_END_X, MENU_ARROW_UP_START_Y, MENU_ARROW_UP_END_Y)) {
    shiftPage(true);
    return(true);
  } else if(p.isTouching(MENU_ARROW_DOWN_START_X, MENU_ARROW_DOWN_END_X, MENU_ARROW_DOWN_START_Y, MENU_ARROW_DOWN_END_Y)) {
    shiftPage(false);
    return(true);
  } else
    return(false);
}

void SubMenu::reset() {
  /*if(_currentPage != _defaultPage)
    setPage(_defaultPage);
  else
    _pages.at(_currentPage).reset();*/
  for(int i = 0; i < _pages.size(); i++)
    _pages.at(i).reset();
}
