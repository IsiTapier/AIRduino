/*
  DisplayV2.cpp - DisplayV2-Library for Librarys.
*/

#include "Arduino.h"
#include "DisplayV2.h"


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

  drawBarChart();
  drawBar();
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

static void DisplayV2::drawBarChart() {
  drawBarBorder();
  drawSections();
}

static void DisplayV2::drawBarBorder() { //x,y,breite, höhe, dicke
  display.fillRect(BORDER_X, BORDER_Y, BORDER_WIDTH, BORDER_HIGHT, WHITE);
  display.fillRect(BAR_X, BAR_Y, BAR_WIDTH, BAR_HIGHT, BLACK); //Setzt das "Loch" in die Mitte des Rechtecks
}

static void DisplayV2::drawSections() {
  first_section_x = map(LIMIT_GOOD, OSV_PPM, LIMIT_BAD, BAR_X, BAR_X + BAR_WIDTH);
  second_section_x = map(LIMIT_MEDIUM, OSV_PPM, LIMIT_BAD, BAR_X, BAR_X + BAR_WIDTH);

  display.drawLine(first_section_x, BAR_Y - 10, first_section_x, BAR_Y + 10 + BAR_HIGHT, GREY);
  display.drawLine(second_section_x, BAR_Y - 10, second_section_x, BAR_Y + 10 + BAR_HIGHT, GREY);
}

static void DisplayV2::drawBar() {
  int barPixel = map(airCondition, 0, LIMIT_BAD, 0, BAR_WIDTH);
  if (airCondition < LIMIT_GOOD)
    display.fillRect(BAR_X + 1, BAR_Y + 1, barPixel, BAR_HIGHT, Util::getColor(state));
  else if (airCondition < LIMIT_MEDIUM)
    display.fillRect(BAR_X + 1, BAR_Y + 1, barPixel, BAR_HIGHT, Util::getColor(state));
  else if (airCondition > LIMIT_MEDIUM) {
    display.fillRect(BAR_X + 1, BAR_Y + 1, barPixel, BAR_HIGHT, Util::getColor(state));
  }
}

static short DisplayV2::first_section_x;
static short DisplayV2::second_section_x;
