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
  Serial.begin(9600);
  Display::setup();

}

static void DisplayV2::loop() {
  state = Meassure::getState();
  airCondition = Meassure::getAirCondition();

  checkState();
  writeInfo();

  //writeAnalogValue();
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
  display.drawRect(BORDER_X, BORDER_Y, BORDER_WIDTH, BORDER_HIGHT, BAR_COLOR);
  //   - search position of the bar -
  //Draw balance bar
  barPixel = map(airCondition, Util::calibration[EEPROM.read(0)].getLowestPPM(), Util::calibration[EEPROM.read(0)].getHighestPPM(), 0, BAR_WIDTH) + BAR_X;

  //display.fillRect(BAR_X, BAR_Y, BAR_WIDTH, BAR_HIGHT, BLACK); //Setzt das "Loch" in die Mitte des Rechtecks
}


static void DisplayV2::drawSections() {
  //    - define section position -
  first_section_x = map(LIMIT_GOOD, Util::calibration[EEPROM.read(0)].getLowestPPM(), Util::calibration[EEPROM.read(0)].getHighestPPM(), 0, BAR_WIDTH) + BAR_X;
  second_section_x = map(LIMIT_MEDIUM, Util::calibration[EEPROM.read(0)].getLowestPPM(), Util::calibration[EEPROM.read(0)].getHighestPPM(), 0, BAR_WIDTH) + BAR_X;


  //    - Draw Sections -
  //display.drawLine(first_section_x, BAR_Y - 10, first_section_x, BAR_Y + 10 + BAR_HIGHT, WHITE);
  //display.drawLine(second_section_x, BAR_Y - 10, second_section_x, BAR_Y + 10 + BAR_HIGHT, WHITE);

  for(short y = BAR_Y - 10; y < BAR_Y + 10 + BAR_HIGHT; y = y+5) {
    for(byte z = 0; z <= BAR_SECTIONS_THICKNESS; z++) {
      display.drawPixel(first_section_x + z, y, BAR_SECTIONS_COLOR);
      display.drawPixel(second_section_x + z, y, BAR_SECTIONS_COLOR);
    }
  }
}

static void DisplayV2::drawBar() {
    //   - search position of the bar -
    barPixel = map(airCondition, Util::calibration[EEPROM.read(0)].getLowestPPM(), Util::calibration[EEPROM.read(0)].getHighestPPM(), 0, BAR_WIDTH);
    if(barPixel <= 0)
      return;
    if(barPixel > BAR_WIDTH)
      barPixel = BAR_WIDTH;

    if (airCondition < LIMIT_GOOD)
      display.fillRect(BAR_X, BAR_Y, barPixel, BAR_HIGHT, Util::getColor(state));
    else if (airCondition < LIMIT_MEDIUM)
      display.fillRect(BAR_X, BAR_Y, barPixel, BAR_HIGHT, Util::getColor(state));
    else if (airCondition > LIMIT_MEDIUM) {
      display.fillRect(BAR_X, BAR_Y, barPixel, BAR_HIGHT, Util::getColor(state));
    }
}

static void DisplayV2::writeAnalogValue() {
    display.fillRect(0, 0, 25, 10, GRAPH_BACKGROUND_COLOR);
    dPrint(0, 0, 1, WHITE, analogRead(A0));
}
