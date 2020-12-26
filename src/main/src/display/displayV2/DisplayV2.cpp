/*
  DisplayV2.cpp - DisplayV2-Library for Librarys.
*/

#include "Arduino.h"
#include "DisplayV2.h"

extern int DisplayV2::barPixel;
extern int DisplayV2::lastBarPixel;
extern short DisplayV2::first_section_x;
extern short DisplayV2::second_section_x;

extern boolean DisplayV2::drop = false;

//   _____      _
//  / ____|    | |
// | (___   ___| |_ _   _ _ __
//  \___ \ / _ \ __| | | | '_ \
//  ____) |  __/ |_| |_| | |_) |
// |_____/ \___|\__|\__,_| .__/
//                        | |
//                       |_|

extern void DisplayV2::setup() {
  Display::setup();
  Serial.println("DisplayV2-Setup started");
  Serial.println("DisplayV2-Setup complete");
  Serial.println();

  //    - define section position -
  first_section_x = map(LIMIT_GOOD, DISPLAYED_PPM_LOWEST, DISPLAYED_PPM_HIGHEST, BAR_START_X, BAR_END_X);
  second_section_x = map(LIMIT_MEDIUM, DISPLAYED_PPM_LOWEST, DISPLAYED_PPM_HIGHEST, BAR_START_X, BAR_END_X);
}

extern void DisplayV2::loop() {

  //showBoxes();
  getData();
  //generateData(400, 1000, 30);

  if(start) {
    drawDisplay();
    drawBarBorder();
  }
  drawBar();
  writeInfo();
  checkState();
  if(start)
    start = false;




  //writeAnalogValue();
  /*if(airCondition == lastAirCondition || airCondition > LIMIT_BAD && lastAirCondition > Util::calibration[EEPROM.read(0)].getHighestPPM())
    return;*/
  //drawBarBorder();


  //showBoxes();
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
  /*for(short z = 0; z < BAR_BORDER_THICKNESS; z++) {
    display.drawRect(BAR_BORDER_X - z, BAR_BORDER_Y - z, BAR_BORDER_LENGTH + z + 2, BAR_BORDER_HEIGHT + z + 2 , BAR_COLOR);
  }

  //   - search position of the bar -
  //Draw balance bar
  barPixel = map(airCondition, Util::calibration[EEPROM.read(0)].getLowestPPM(), Util::calibration[EEPROM.read(0)].getHighestPPM(), 0, BAR_LENGTH) + BAR_START_X;
  display.fillRect(barPixel + 1, BAR_BORDER_Y, BAR_LENGTH - barPixel, BAR_HEIGHT, BACKGROUND_COLOR);
  //display.fillRect(BAR_START_X, BAR_Y, BAR_WIDTH, BAR_HIGHT, BLACK); //Setzt das "Loch" in die Mitte des Rechtecks*/
  //display.fillRect(BAR_BORDER_X, BAR_BORDER_Y, BAR_BORDER_LENGTH, BAR_BORDER_HEIGHT, BAR_COLOR);

  //drawBorder(BAR_BORDER_X, BAR_BORDER_Y, BAR_BORDER_LENGTH, BAR_BORDER_HEIGHT, BAR_BORDER_THICKNESS, WHITE);
  //display.fillRect(BAR_START_X, BAR_Y, BAR_LENGTH, BAR_HEIGHT, BACKGROUND_COLOR);
  drawBorder(BAR_BORDER_X, BAR_BORDER_Y, BAR_BORDER_LENGTH, BAR_BORDER_HEIGHT, BAR_BORDER_THICKNESS, WHITE);
}


extern void DisplayV2::drawSections() {



  //    - Draw Sections -
  //display.drawLine(first_section_x, BAR_Y - 10, first_section_x, BAR_Y + 10 + BAR_HIGHT, WHITE);
  //display.drawLine(second_section_x, BAR_Y - 10, second_section_x, BAR_Y + 10 + BAR_HIGHT, WHITE);

  for(short y = BAR_Y - 10; y < BAR_Y + 10 + BAR_HEIGHT; y = y+5) {
    for(byte z = 0; z <= BAR_SECTIONS_THICKNESS; z++) {
      display.drawPixel(first_section_x + z, y, BAR_SECTIONS_COLOR);
      display.drawPixel(second_section_x + z, y, BAR_SECTIONS_COLOR);
    }
  }
}

extern void DisplayV2::drawBar() {
    //   - search position of the bar -
    barPixel = map(airCondition, DISPLAYED_PPM_LOWEST, DISPLAYED_PPM_HIGHEST, BAR_START_X, BAR_END_X);
    lastBarPixel = map(lastAirCondition, DISPLAYED_PPM_LOWEST, DISPLAYED_PPM_HIGHEST, BAR_START_X, BAR_END_X);
    if(barPixel <= BAR_START_X && lastAirCondition <= BAR_START_X || barPixel == lastBarPixel)
      return;
    if(barPixel > BAR_END_X)
      barPixel = BAR_END_X;
    if(lastBarPixel > BAR_END_X)
      lastBarPixel = BAR_END_X;

    Serial.println();
    Serial.println(airCondition);
    Serial.println(barPixel);
    Serial.println();
    Serial.println(lastAirCondition);
    Serial.println(lastBarPixel);
    Serial.println();
    //RELIEABLY
    //TODO: vereinfachen
    if(COLOR_MODE) {
      if(airCondition >= LIMIT_GOOD) {
        if(!(lastAirCondition >= LIMIT_GOOD))
          display.fillRect(BAR_START_X, BAR_Y, first_section_x - BAR_START_X, BAR_HEIGHT, PPM_COLOR_N);
        if(airCondition >= LIMIT_MEDIUM) {
          if(!(lastAirCondition >= LIMIT_MEDIUM))
            display.fillRect(first_section_x, BAR_Y, second_section_x - first_section_x, BAR_HEIGHT, PPM_COLOR_R);
          if(airCondition >= DISPLAYED_PPM_HIGHEST) {
            if(lastAirCondition < DISPLAYED_PPM_HIGHEST)
              display.fillRect(second_section_x, BAR_Y, BAR_END_X - second_section_x, BAR_HEIGHT, PPM_COLOR_A);
          } else if(lastAirCondition < airCondition)
            display.fillRect(second_section_x, BAR_Y, barPixel - second_section_x, BAR_HEIGHT, PPM_COLOR_A);
          else
            display.fillRect(barPixel, BAR_Y, lastBarPixel - barPixel, BAR_HEIGHT, CHART_BACKGROUND_COLOR);
        } else if(lastAirCondition < airCondition)
            display.fillRect(first_section_x, BAR_Y, barPixel - first_section_x, BAR_HEIGHT, PPM_COLOR_R);
          else
            display.fillRect(barPixel, BAR_Y, lastBarPixel - barPixel, BAR_HEIGHT, CHART_BACKGROUND_COLOR);
      } else if(lastAirCondition < airCondition)
          display.fillRect(BAR_START_X, BAR_Y, barPixel - BAR_START_X, BAR_HEIGHT, PPM_COLOR_N);
        else
          display.fillRect(barPixel, BAR_Y, lastBarPixel - barPixel, BAR_HEIGHT, CHART_BACKGROUND_COLOR);
    } else if(lastState == state && !start || state >= 2 && 2 <= lastState && !start) {
      if(lastAirCondition < airCondition)
        display.fillRect(lastBarPixel, BAR_Y, barPixel - lastBarPixel, BAR_HEIGHT, state.getColor(COLORED_CHART));
      else
        display.fillRect(barPixel, BAR_Y, lastBarPixel - barPixel, BAR_HEIGHT, CHART_BACKGROUND_COLOR);
    } else {
      if(lastAirCondition > airCondition)
        display.fillRect(barPixel, BAR_Y, lastBarPixel - barPixel, BAR_HEIGHT, CHART_BACKGROUND_COLOR);
      display.fillRect(BAR_START_X, BAR_Y, barPixel - BAR_START_X, BAR_HEIGHT, state.getColor(COLORED_CHART));
    }


  //incredibly short and not quite working
/*  if(Util::getStateOf(lastAirCondition) == Util::getStateOf(airCondition) && lastAirCondition > 0 || Util::getStateOf(airCondition) >= 2 && 2 <= Util::getStateOf(lastAirCondition) || COLOR_MODE) {
    if(lastAirCondition < airCondition)
      display.fillRect(lastBarPixel, BAR_Y, barPixel - lastBarPixel, BAR_HEIGHT, state.getColor(COLORED_CHART));
    else
      display.fillRect(barPixel, BAR_Y, lastBarPixel - barPixel, BAR_HEIGHT, CHART_BACKGROUND_COLOR);
  } else {
    display.fillRect(barPixel, BAR_Y, lastBarPixel - barPixel, BAR_HEIGHT, CHART_BACKGROUND_COLOR);
    display.fillRect(BAR_START_X, BAR_Y, barPixel - BAR_START_X, BAR_HEIGHT, state.getColor(COLORED_CHART));
  }*/
  drawSections();
}

extern void DisplayV2::writeAnalogValue() {
    display.fillRect(0, 0, 25, 10, BACKGROUND_COLOR);
    dPrint(0, 0, 1, WHITE, analogRead(A0));
}
