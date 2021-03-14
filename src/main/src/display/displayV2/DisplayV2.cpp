/*
  DisplayV2.cpp - DisplayV2-Library for Librarys.
*/

#include "Arduino.h"
#include "DisplayV2.h"

extern int DisplayV2::barPixel;
extern int DisplayV2::lastBarPixel;

/*
   _____      _
  / ____|    | |
 | (___   ___| |_ _   _ _ __
  \___ \ / _ \ __| | | | '_ \
  ____) |  __/ |_| |_| | |_) |
 |_____/ \___|\__|\__,_| .__/
                        | |
                       |_|
*/

void DisplayV2::setup() {
  DisplayVX::setup();
  debug(DEBUG, SETUP, "DisplayV2 SETUP started");
  debug(DEBUG, SETUP, "DisplayV2 SETUP completed");
  debug(DEBUG, SETUP, "");
  if(general::mode.equaled(LOADINGSCREEN)) {
    loop();
    start = false;
  }
}

void DisplayV2::loop() {
  if(start) {
    drawDisplay();
    drawBarBorder();
    DisplayVX::handleData();
    if(general::mode.equals(CHART)) {
      DisplayVX::writeInfo();
      DisplayVX::checkState();
    }
  } else {
      if(general::mode.equals(CHART) && general::mode.equaled(CHART)) {
        DisplayVX::writeInfo();
        DisplayVX::checkState();
        }
    }
 // DisplayVX::loop();
  drawBar();
  //end setup
  if(start)
    start = false;
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




void DisplayV2::drawBarBorder() { //x,y,breite, höhe, dicke
  /*for(short z = 0; z < BAR_BORDER_THICKNESS; z++) {
    display.drawRect(BAR_BORDER_X - z, BAR_BORDER_Y - z, BAR_BORDER_LENGTH + z + 2, BAR_BORDER_HEIGHT + z + 2 , BAR_COLOR);
  }

  //   - search position of the bar -
  //Draw balance bar
  barPixel = map(airCondition, calibration[EEPROM.read(0)].getLowestPPM(), calibration[EEPROM.read(0)].getHighestPPM(), 0, BAR_LENGTH) + BAR_START_X;
  display.fillRect(barPixel + 1, BAR_BORDER_Y, BAR_LENGTH - barPixel, BAR_HEIGHT, BACKGROUND_COLOR);
  //display.fillRect(BAR_START_X, BAR_Y, BAR_WIDTH, BAR_HIGHT, BLACK); //Setzt das "Loch" in die Mitte des Rechtecks*/
  //display.fillRect(BAR_BORDER_X, BAR_BORDER_Y, BAR_BORDER_LENGTH, BAR_BORDER_HEIGHT, BAR_COLOR);

  //drawBorder(BAR_BORDER_X, BAR_BORDER_Y, BAR_BORDER_LENGTH, BAR_BORDER_HEIGHT, BAR_BORDER_THICKNESS, WHITE);
  //display.fillRect(BAR_START_X, BAR_Y, BAR_LENGTH, BAR_HEIGHT, BACKGROUND_COLOR);
  drawBorder(BAR_BORDER_X, BAR_BORDER_Y, BAR_BORDER_LENGTH, BAR_BORDER_HEIGHT, BAR_BORDER_THICKNESS, CHART_BORDER_COLOR);
  drawSections();
}


void DisplayV2::drawSections() {
  //    - Draw Sections -
  //display.drawLine(FIRST_SECTION_X, BAR_Y - 10, FIRST_SECTION_X, BAR_Y + 10 + BAR_HIGHT, WHITE);
  //display.drawLine(SECOND_SECTION_X, BAR_Y - 10, SECOND_SECTION_X, BAR_Y + 10 + BAR_HIGHT, WHITE);
  /*for(short y = BAR_Y - 10; y < BAR_Y + 10 + BAR_HEIGHT; y = y+5) {
    for(byte z = 0; z <= BAR_SECTIONS_THICKNESS; z++) {
      display.drawPixel(FIRST_SECTION_X + z, y, BAR_SECTIONS_COLOR);
      display.drawPixel(SECOND_SECTION_X + z, y, BAR_SECTIONS_COLOR);
    }
  }*/
  drawLine(FIRST_SECTION_START_X, BAR_SECTIONS_Y, BAR_SECTIONS_LENGTH, BAR_SECTIONS_HEIGHT, BAR_SECTIONS_COLOR, 1, BAR_SECTIONS_STRIPE_DISTANCE);
  drawLine(SECOND_SECTION_START_X, BAR_SECTIONS_Y, BAR_SECTIONS_LENGTH, BAR_SECTIONS_HEIGHT, BAR_SECTIONS_COLOR, 1, BAR_SECTIONS_STRIPE_DISTANCE);
}

void DisplayV2::drawBar() {
  //   - search position of the bar -
  barPixel = map(airCondition, DISPLAYED_PPM_LOWEST, DISPLAYED_PPM_HIGHEST, BAR_START_X, BAR_END_X);
  lastBarPixel = map(lastAirCondition, DISPLAYED_PPM_LOWEST, DISPLAYED_PPM_HIGHEST, BAR_START_X, BAR_END_X);
  if((barPixel <= BAR_START_X && lastAirCondition <= BAR_START_X) || (barPixel == lastBarPixel && !start))
    return;
  if(barPixel < BAR_START_X)
    barPixel = BAR_START_X;
  if(barPixel > BAR_END_X)
    barPixel = BAR_END_X;
  if(lastBarPixel > BAR_END_X)
    lastBarPixel = BAR_END_X;
  if(lastBarPixel < BAR_START_X)
    lastBarPixel = BAR_START_X;

  //TODO: vereinfachen
  if(COLOR_MODE && state != VENTILATING) {
    if(airCondition >= LIMIT_GOOD) {
      if(!(lastAirCondition >= LIMIT_GOOD) || lastState == VENTILATING)
        display.fillRect(BAR_START_X, BAR_Y, FIRST_SECTION_X - BAR_START_X, BAR_HEIGHT, COLOR_STATUS_NORMAL);
      if(airCondition >= LIMIT_MEDIUM) {
        if(!(lastAirCondition >= LIMIT_MEDIUM) || lastState == VENTILATING)
          display.fillRect(FIRST_SECTION_X, BAR_Y, SECOND_SECTION_X - FIRST_SECTION_X, BAR_HEIGHT, COLOR_STATUS_RISK);
        if(airCondition >= DISPLAYED_PPM_HIGHEST) {
          if(lastAirCondition < DISPLAYED_PPM_HIGHEST || lastState == VENTILATING)
            display.fillRect(SECOND_SECTION_X, BAR_Y, BAR_END_X - SECOND_SECTION_X, BAR_HEIGHT, COLOR_STATUS_ALARM);
        } else if(lastAirCondition < airCondition)
          display.fillRect(SECOND_SECTION_X, BAR_Y, barPixel - SECOND_SECTION_X, BAR_HEIGHT, COLOR_STATUS_ALARM);
        else
          display.fillRect(barPixel, BAR_Y, lastBarPixel - barPixel, BAR_HEIGHT, CHART_BACKGROUND_COLOR);
      } else if(lastAirCondition < airCondition)
        display.fillRect(FIRST_SECTION_X, BAR_Y, barPixel - FIRST_SECTION_X, BAR_HEIGHT, COLOR_STATUS_RISK);
      else
        display.fillRect(barPixel, BAR_Y, lastBarPixel - barPixel, BAR_HEIGHT, CHART_BACKGROUND_COLOR);
    } else if(lastAirCondition < airCondition)
      display.fillRect(BAR_START_X, BAR_Y, barPixel - BAR_START_X, BAR_HEIGHT, COLOR_STATUS_NORMAL);
    else
      display.fillRect(barPixel, BAR_Y, lastBarPixel - barPixel, BAR_HEIGHT, CHART_BACKGROUND_COLOR);
  } else if(state.getColor(COLORED_CHART) == lastState.getColor(COLORED_CHART) && !start) {
    if(lastAirCondition < airCondition) {
      display.fillRect(lastBarPixel, BAR_Y, barPixel - lastBarPixel, BAR_HEIGHT, state.getColor(COLORED_CHART));
    } else {
      display.fillRect(barPixel, BAR_Y, lastBarPixel - barPixel, BAR_HEIGHT, CHART_BACKGROUND_COLOR);
    }
  } else {
    if(lastAirCondition > airCondition) 
      display.fillRect(barPixel, BAR_Y, lastBarPixel - barPixel, BAR_HEIGHT, CHART_BACKGROUND_COLOR);
    display.fillRect(BAR_START_X, BAR_Y, barPixel - BAR_START_X, BAR_HEIGHT, state.getColor(COLORED_CHART));
  }
  drawSections();
}

