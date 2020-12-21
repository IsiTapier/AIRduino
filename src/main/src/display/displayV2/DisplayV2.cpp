/*
  DisplayV2.cpp - DisplayV2-Library for Librarys.
*/

#include "Arduino.h"
#include "DisplayV2.h"

extern short DisplayV2::first_section_x;
extern short DisplayV2::second_section_x;
extern int DisplayV2::barPixel;

//   _____      _
//  / ____|    | |
// | (___   ___| |_ _   _ _ __
//  \___ \ / _ \ __| | | | '_ \
//  ____) |  __/ |_| |_| | |_) |
// |_____/ \___|\__|\__,_| .__/
//                       | |
//                       |_|

extern void DisplayV2::setup() {
  Display::setup();
  Serial.println("DisplayV2-Setup started");
  Serial.println("DisplayV2-Setup complete");
  Serial.println();
}

extern void DisplayV2::loop() {
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




extern void DisplayV2::drawBarBorder() { //x,y,breite, höhe, dicke
  Util::debug("Border x", BORDER_X);
  for(short z = 0; z < BORDER_THICKNESS; z++) {
    display.drawRect(BORDER_X - z, BORDER_Y - z, BORDER_WIDTH + z + 2, BORDER_HIGHT + z + 2 , BAR_COLOR);
  }

  //   - search position of the bar -
  //Draw balance bar
  barPixel = map(airCondition, Util::calibration[EEPROM.read(0)].getLowestPPM(), Util::calibration[EEPROM.read(0)].getHighestPPM(), 0, BAR_WIDTH) + BAR_X;
  display.fillRect(barPixel + 1, BAR_Y, (BAR_X + BAR_WIDTH) - barPixel, BAR_HIGHT, GRAPH_BACKGROUND_COLOR);
  //display.fillRect(BAR_X, BAR_Y, BAR_WIDTH, BAR_HIGHT, BLACK); //Setzt das "Loch" in die Mitte des Rechtecks
}


extern void DisplayV2::drawSections() {
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

extern void DisplayV2::drawBar() {
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

extern void DisplayV2::writeAnalogValue() {
    display.fillRect(0, 0, 25, 10, GRAPH_BACKGROUND_COLOR);
    dPrint(0, 0, 1, WHITE, analogRead(A0));
}
