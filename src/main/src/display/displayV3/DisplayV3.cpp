/*
 DisplayV3.cpp - DisplayV3-Library for Librarys.
*/

#include "DisplayV3.h"


int DisplayV3::stateBackgroundColor;
String lastStateTitle = "";

void DisplayV3::setup() {
    DisplayVX::setup();
    debug(DEBUG, SETUP, "DisplayV3 SETUP started");
    debug(DEBUG, SETUP, "DisplayV3 SETUP completed");
    debug(DEBUG, SETUP, "");
    display.fillScreen(BLACK);
    dPrint(lastState.getTitle(), DISPLAY_LENGTH/2, STATUS_MARGIN_TOP+25, 4, BLACK, 1);
    dPrint(state.getTitle(), DISPLAY_LENGTH/2, STATUS_MARGIN_TOP+25, 4, GREY, 1);  
}

void DisplayV3::loop() {
  	stateBackgroundColor = state.getBackgroundColor();
    writeInfoV3();
    DisplayVX::handleData();
    
    DisplayVX::checkState();
}

void DisplayV3::writeInfoV3() {
    //Titles
    if (lastState != state) {
      dPrint(lastState.getTitle(), DISPLAY_LENGTH/2, STATUS_MARGIN_TOP+25, 4, BLACK, 1);
      dPrint(state.getTitle(), DISPLAY_LENGTH/2, STATUS_MARGIN_TOP+25, 4, GREY, 1);
    }
    
    DisplayVX::drawPPMStraight(157, 125, PPM_SIZE+1, state.getColor(true), 7);

    //Draw Bar
    if(lastAirCondition != airCondition || start) {
      //bar on the bottom
      short x = map(airCondition, LIMIT_GOOD, LIMIT_PIEP, 0, DISPLAY_LENGTH);
      display.fillRect(0, 220, x, 20, state.getColor(true));
      display.fillRect(x+1, 220, DISPLAY_LENGTH-x, 20, BLACK);
      display.fillCircle(x, DISPLAY_HEIGHT, 20, state.getColor());

      //bar on the top
      display.fillRect(0, 0, x, 20, state.getColor(true));
      display.fillRect(x+1, 0, DISPLAY_LENGTH-x, 20, BLACK);
      display.fillCircle(x, 0, 19, state.getColor(true));

      //draw ICONS
      display.pushImage(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, homeDark, WHITE);
      display.pushImage(MENU_ARROW_RESET_START_X, MENU_ARROW_RESET_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, arrowResetDark, BLACK);
    }
    //calculate time since last ventilating
    long startTime = Meassure::getStartTime();
    seconds = (millis() - startTime) / 1000 % 60;
    minutes = ((millis() - startTime) / 1000 - seconds) / 60;
    //create String
    time = "";
    if (minutes < 10) {
      time = time + 0;
    }
    time = time + minutes;
    time = time + ":";
    if (seconds < 10)
      time = time + 0;
    time = time + seconds;

    //Clear old Pixels
    //dPrint(lasttime, timeR_X, timeR_Y, timeR_SIZE, BAR_BACKGROUND_COLOR, 8);
    //write new Pixels
    if(minutes <= 20) {
      dPrint(time, 165, 130, 10, WHITE, 1, BLACK, lastTime, -1, (seconds == 0 && (minutes == 0 || (minutes == 20 && COLORED_TIME))) || start || state.getColor() != lastState.getColor());
    } else {
      dPrint(time, 165, 130, 10, RED, 1, BLACK, lastTime, -1, (seconds == 0 && (minutes == 0 || (minutes == 20 && COLORED_TIME))) || start || state.getColor() != lastState.getColor());
    }
    if((minutes == 20) && (seconds == 0)) {
      dPrint(time, 165, 130, 10, RED, 1, BLACK, lastTime, -1);
    }
    
    //Set new lasttime
    lastTime = time; //Setzt letzten Werts
}
