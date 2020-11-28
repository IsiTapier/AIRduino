/*
  DisplayV2.cpp - DisplayV2-Library for Librarys.
*/

#include "Arduino.h"
#include "DisplayV2.h"

static short DisplayV2::first_section_x;
static short DisplayV2::second_section_x;
static int DisplayV2::barPixel;

//   _____      _
//  / ____|    | |
// | (___   ___| |_ _   _ _ __
//  \___ \ / _ \ __| | | | '_ \
//  ____) |  __/ |_| |_| | |_) |
// |_____/ \___|\__|\__,_| .__/
//                       | |
//                       |_|

static void DisplayV2::setup() {
  Display::setup();

}

static void DisplayV2::loop() {
  state = Meassure::getState();
  airCondition = Meassure::getAirCondition();

  checkState();
  writeInfo();

  writeAnalogValue();
  drawBarBorder();
  drawBar();
  drawSections();

}


//  _____  _           _
// |  __ \(_)         | |
// | |  | |_ ___ _ __ | | __ _ _   _
// | |  | | / __| '_ \| |/ _` | | | |
// | |__| | \__ \ |_) | | (_| | |_| |
// |_____/|_|___/ .__/|_|\__,_|\__, |
//              | |             __/ |
//              |_|            |___/
//




static void DisplayV2::drawBarBorder() { //x,y,breite, höhe, dicke
  display.fillRect(BORDER_X, BORDER_Y, BORDER_WIDTH, BORDER_HIGHT, GREY);
  //   - search position of the bar -
  //Draw balance bar
  barPixel = map(airCondition, Util::calibration[EEPROM.read(0)].getLowestPPM(), Util::calibration[EEPROM.read(0)].getHighestPPM(), BAR_X, BAR_X + BAR_WIDTH);
  display.fillRect(barPixel + 1, BAR_Y, (BAR_X + BAR_WIDTH) - barPixel, BAR_HIGHT, GRAPH_BACKGROUND_COLOR);

  //display.fillRect(BAR_X, BAR_Y, BAR_WIDTH, BAR_HIGHT, BLACK); //Setzt das "Loch" in die Mitte des Rechtecks
}


static void DisplayV2::drawSections() {
  //    - define section position -
  first_section_x = map(LIMIT_GOOD, Util::calibration[EEPROM.read(0)].getLowestPPM(), Util::calibration[EEPROM.read(0)].getHighestPPM(), 0, BAR_WIDTH) + BAR_X;
  second_section_x = map(LIMIT_MEDIUM, Util::calibration[EEPROM.read(0)].getLowestPPM(), Util::calibration[EEPROM.read(0)].getHighestPPM(), 0, BAR_WIDTH) + BAR_X;


  //    - Draw Sections -
  //display.drawLine(first_section_x, BAR_Y - 10, first_section_x, BAR_Y + 10 + BAR_HIGHT, WHITE);
  //display.drawLine(second_section_x, BAR_Y - 10, second_section_x, BAR_Y + 10 + BAR_HIGHT, WHITE);

  for(short y = BAR_Y - 10; y < BAR_Y + 10 + BAR_HIGHT; y++) {
    if(y%5 == 0) {
      display.drawPixel(first_section_x, y, WHITE);
      display.drawPixel(second_section_x, y, WHITE);
      display.drawPixel(first_section_x + 1, y, WHITE);
      display.drawPixel(second_section_x + 1, y, WHITE);
    }
  }

}

static void DisplayV2::drawBar() {
  if(barPixel <= BAR_WIDTH && barPixel >= 0) {
    //   - search position of the bar -
    barPixel = map(airCondition, Util::calibration[EEPROM.read(0)].getLowestPPM(), Util::calibration[EEPROM.read(0)].getHighestPPM(), BAR_X, BAR_X + BAR_WIDTH);

    if (airCondition < LIMIT_GOOD)
      display.fillRect(BAR_X, BAR_Y, barPixel, BAR_HIGHT, Util::getColor(state));
    else if (airCondition < LIMIT_MEDIUM)
      display.fillRect(BAR_X, BAR_Y, barPixel, BAR_HIGHT, Util::getColor(state));
    else if (airCondition > LIMIT_MEDIUM) {
      display.fillRect(BAR_X, BAR_Y, barPixel, BAR_HIGHT, Util::getColor(state));
    }
  }
}

static void DisplayV2::writeAnalogValue() {
    display.fillRect(0, 0, 15, 10, GRAPH_BACKGROUND_COLOR);
    dPrint(0, 0, 1, WHITE, analogRead(A0));
}
