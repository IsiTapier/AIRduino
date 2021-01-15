/*
  MenuPage.h - MenuPage-Library for Airduino.
*/

#ifndef MenuPage_h
#define MenuPage_h

#include "Arduino.h"
#include "../../../classes/slider/Slider.h"
#include "../subMenu/SubMenu.h"

class MenuPage : subMenu {
  public:
    MenuPage(int number) //TODO types
    void drawPage();
    void checkTouch(TSPoint p);

  private:
    void init();
    void drawSide();

    int page;
    
    Slider firstSection;
    Slider secondSection;
    Slider thirdSection;
    Slider fourthSection;
    Slider fifthSection;
    Slider sixthSection;

};

#endif
