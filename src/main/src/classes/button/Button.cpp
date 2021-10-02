/*
 Button.cpp - Button-Library for Librarys.
*/

#include "Button.h"
#include "../../display/menu/Menu.h"
#include "../../display/Display.h"

  Button::Button(void) {}

  Button::Button(Setting* setting) {
    _setting = setting;
  }

  Button& Button::setSection(byte section) {
    _section = section;
    return(*this);
  }

  void Button::init() {
    write();
  }

  void Button::checkTouch(TSPoint p) {
    if(p.isTouching(0, MENU_SECTION_LENGTH, MENU_SECTION_Y(_section)+5, (MENU_SECTION_Y(_section) + MENU_SECTION_HEIGHT))) {
      execute();
       // TODO universal
      if(strlen(_setting->getDebugMessage()) != 0)
        debug(DEBUG, MENUD, _setting->getDebugMessage());
    }
  }

  void Button::write() {
    String text = _setting->getTitle();
    dPrint(text, MENU_SECTION_LENGTH/2, MENU_SECTION_TEXT_Y(_section), MENU_SECTION_SIZE, TEXT_COLOR, 4);
  }
  
  void Button::execute() {
    switch(_setting->getDefaultValue()) {
      case 0: ts.calibration(); break;
      case 1: Meassure::calibrateMin(); break;
      case 2: Meassure::calibrateMax(); break;
      case 3: Meassure::autoCalibrationMin(); break;
      case 4: Menu::openReportMenu(); break;
      case 5: reportBug("Sensor Fehler"); break; 
      case 6: reportBug("Komische Werte"); break;
      case 7: reportBug("Graphische Fehler"); break; 
      case 8: reportBug("Ton Problem"); break; 
      case 9: reportBug("Schnelle Hilfe ben" +oe+ "tigt"); break; 
      case 10: requestDecision("ea.jcbs@gmail.com", "wir bitten um Feedback", "", "Zur" +ue+ "ck"); break; 
      case 11: Display::drawInfoScreen(5000); break; 
      case 12: reconnectSystem(); break;
      case 13: Display::screenShot(); break;
      default: break;
    }

  }

