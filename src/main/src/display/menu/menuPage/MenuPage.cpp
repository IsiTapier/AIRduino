/*
 MenuPage.cpp - MenuPage-Library for Librarys.
*/

#include "MenuPage.h"

extern Slider firstSection(0, "Version 1", "Version2", false, true, "Version changed");
extern Slider secondSection(1, "test1", "test2");
extern Slider thirdSection(2, "test1", "test2");
extern Slider fourthSection(3, "test1", "test2");
extern Slider fifthSection(4, "test1", "test2");
extern Slider sixthSection(5, "test1", "test2");
extern int page;


extern MenuPage::MenuPage(int number) {
  page = number;
}

extern void MenuPage::drawPage() {
  init();
  drawSide();
}

extern void MenuPage::init() {
  firstSection.init();
  secondSection.init();
  thirdSection.init();
  fourthSection.init();
  fifthSection.init();
  sixthSection.init();
}

extern void MenuPage::drawSide() {

}

extern void MenuPage::checkTouch(TSPoint p) {
  firstSection.checkTouch(p);
  secondSection.checkTouch(p);
  thirdSection.checkTouch(p);
  fourthSection.checkTouch(p);
  fifthSection.checkTouch(p);
  sixthSection.checkTouch(p);
}
