/*
 DisplayV3.cpp - DisplayV3-Library for Librarys.
*/

#include "DisplayV3.h"


int DisplayV3::stateBackgroundColor;

void DisplayV3::setup() {
    DisplayVX::setup();
    debug(DEBUG, SETUP, "DisplayV3 SETUP started");
    debug(DEBUG, SETUP, "DisplayV3 SETUP completed");
    debug(DEBUG, SETUP, "");

    if(general::mode.equaled(LOADINGSCREEN)) {
        loop();
    start = false;
    }
}

void DisplayV3::loop() {
  	stateBackgroundColor = state.getBackgroundColor();
    if(start) {
        drawDisplay();
        //Draw Bar
        display.fillRect(0, 0, DISPLAY_LENGTH, 20, state.getColor(true));
        display.fillRect(0, 220, DISPLAY_LENGTH, 20, state.getColor(true));
        //draw Icons
        display.pushImage(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, gear, WHITE);
        display.pushImage(MENU_ARROW_RESET_START_X, MENU_ARROW_RESET_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, arrowResetDark, WHITE);

        DisplayVX::handleData();
        if(general::mode.equals(CHART)) {
            writeInfoV3();
            DisplayVX::checkState();
        }

    } else {
      if(general::mode.equals(CHART) && general::mode.equaled(CHART)) {
        writeInfoV3();
        DisplayVX::checkState();
        }
    }
    if(lastState.getColor() != state.getColor()) {
        drawDisplay();
        writeInfoV3();
    }

    //end setup
    if(start)
        start = false;
}

 void DisplayV3::drawDisplay() {
    display.fillScreen(BLACK);
    display.pushImage(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, gear, WHITE);
    debug(INFO, SETUP, "Display drawn");
    // dPrint("16:44", 160, 120, 11, BLACK, 4, state.getColor(true));
}

void DisplayV3::writeInfoV3() {
    //ppm zeichnen
    String lastppm;
    if(ERRORLAST)
      lastppm = "error";
    else if(lastAirCondition < 10)
      lastppm = "  " + String(lastAirCondition);
    else if(lastAirCondition < 100)
      lastppm = " " + String(lastAirCondition);
    else
      lastppm = String(lastAirCondition);
    String ppm;
    if(ERROR)
      ppm = "error";
    else if(airCondition <= 0)
      ppm = lastppm;
    else if(airCondition < 10)
      ppm = "  " + String(airCondition);
    else if(airCondition < 100)
      ppm = " " + String(airCondition);
    else
      ppm = String(airCondition);
    if (lastState != state || start) {
      //Titles
      dPrint(lastState.getTitle(), DISPLAY_LENGTH/2, STATUS_MARGIN_TOP+25, 4, BLACK, 1);
      dPrint(state.getTitle(), DISPLAY_LENGTH/2, STATUS_MARGIN_TOP+25, 4, GREY, 1);


      //Draw PPM
      dPrint(lastppm, 157, 125, ERRORLAST ? PPM_SIZE-1+1 : PPM_SIZE+1, BLACK, 7);
      dPrint(ppm, 157, 125, ERROR ? PPM_SIZE-1+1 : PPM_SIZE+1, state.getColor(true), 7);
    }

    //Draw Bar
    if(lastppm != ppm) {
      //bar on the bottom
      short x = map(airCondition, LIMIT_GOOD, LIMIT_PIEP, 0, DISPLAY_LENGTH);
      display.fillRect(0, 220, x, 20, state.getColor(true));
      display.fillRect(x+1, 220, DISPLAY_LENGTH-x, 20, BLACK);
      display.fillCircle(x, DISPLAY_HEIGHT, 20, state.getColor(true));

      //bar on the top
      display.fillRect(0, 0, x, 20, state.getColor(true));
      display.fillRect(x+1, 0, DISPLAY_LENGTH-x, 20, BLACK);
      display.fillCircle(x, 0, 19, state.getColor(true));

      //draw ICONS
      display.pushImage(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, gear, WHITE);
      display.pushImage(MENU_ARROW_RESET_START_X, MENU_ARROW_RESET_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, arrowResetLight, WHITE);
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

    Serial.println(ppm);
    //Clear old Pixels
    //dPrint(lasttime, timeR_X, timeR_Y, timeR_SIZE, BAR_BACKGROUND_COLOR, 8);
    //write new Pixels
    dPrint(time, 165, 130, 10, WHITE, 1, BLACK, lastTime, -1, (seconds == 0 && (minutes == 0 || (minutes == 20 && COLORED_TIME))) || start || state.getColor() != lastState.getColor());

    //Set new lasttime
    lastTime = time; //Setzt letzten Wert
    lastError = ERRORLAST;
}
