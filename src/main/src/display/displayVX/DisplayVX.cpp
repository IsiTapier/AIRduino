/*
  DisplayVX.cpp - DisplayVX-Library for Librarys.
*/

#include "Arduino.h"
#include "DisplayVX.h"

extern State DisplayVX::state;
extern State DisplayVX::lastState;
extern int DisplayVX::airCondition;
extern int DisplayVX::lastAirCondition = 0;
extern int DisplayVX::blinkSwitch = 0;
extern int DisplayVX::statusLetters;
extern String DisplayVX::statusInfo;
extern String DisplayVX::lastTime;
extern String DisplayVX::time;
extern short DisplayVX::seconds;
extern short DisplayVX::minutes;
extern boolean DisplayVX::start;
extern boolean DisplayVX::drop = false;



  //   _____      _
  //  / ____|    | |
  // | (___   ___| |_ _   _ _ __
  //  \___ \ / _ \ __| | | | '_ \
  //  ____) |  __/ |_| |_| | |_) |
  // |_____/ \___|\__|\__,_| .__/
  //                       | |
  //                       |_|


  void DisplayVX::setup() {
    debug(DEBUG, SETUP, "DisplayVX SETUP started");
    start = true;
    debug(DEBUG, SETUP, "DisplayVX SETUP completed");
    debug(DEBUG, SETUP, "");
  }

  void DisplayVX::loop(boolean draw) {
    if(general::data.getValue())
      getData();
    else
      generateData(400, 1100, 1);
    //info
    if(draw) {
      writeInfo();
      checkState();
    }
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

  void DisplayVX::drawDisplay() {
    display.fillScreen(BACKGROUND_COLOR);
    display.fillRect(0, DATABOX_Y, DISPLAY_LENGTH, DATABOX_HEIGHT, DATABOX_BACKGROUND_COLOR);
    display.pushImage(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, gear, WHITE);
    display.fillRect(0, TOP_BAR_HEIGHT, DISPLAY_LENGTH, TOP_BAR_THICKNESS, TEXT_COLOR);
    debug(INFO, SETUP, "Display drawn");
  }

  void DisplayVX::getData() {
    lastState = state;
    lastAirCondition = airCondition;
    state = Meassure::getState();
    airCondition = Meassure::getAirCondition();
  }

  void DisplayVX::generateData(int startPPM, int endPPM, int changePPM) {
    lastState = state;
    lastAirCondition = airCondition;
    if(start)
      airCondition = startPPM;
    if(drop)
      airCondition -= changePPM;
    else
      airCondition += changePPM;
    if(airCondition >= endPPM)
      drop = true;
    if(airCondition <= startPPM)
      drop = false;
    state = getStateOf(airCondition);
  }

  void DisplayVX::checkState() {
    if (blinkSwitch == 1) {
      //restore Border
      drawBorder(0, 0, DISPLAY_LENGTH, DISPLAY_HEIGHT, 1, BACKGROUND_COLOR);
      display.drawLine(0, DISPLAY_HEIGHT-1, DISPLAY_LENGTH-1, DISPLAY_HEIGHT-1, DATABOX_BACKGROUND_COLOR);
      display.drawLine(0, DATABOX_Y, 0, DISPLAY_HEIGHT-1, DATABOX_BACKGROUND_COLOR);
      display.drawLine(DISPLAY_LENGTH-1, DATABOX_Y, DISPLAY_LENGTH-1, DISPLAY_HEIGHT-1, DATABOX_BACKGROUND_COLOR);
      display.drawLine(0, DATABOX_BAR_Y, 0, DATABOX_Y-1, state.getColor(COLORED_BAR));
      display.drawLine(DISPLAY_LENGTH-1, DATABOX_BAR_Y, DISPLAY_LENGTH-1, DATABOX_Y-1, state.getColor(COLORED_BAR));
      display.drawLine(0, TOP_BAR_HEIGHT, 0, STATUS_END_HEIGHT-1, state.getColor(COLORED_BAR));
      display.drawLine(DISPLAY_LENGTH-1, TOP_BAR_HEIGHT, DISPLAY_LENGTH-1, STATUS_END_HEIGHT-1, state.getColor(COLORED_BAR));
      digitalWrite(PIEZO, LOW);
      if(state < 3)
        blinkSwitch = 0;
    } else if(blinkSwitch == 11) {
      if(general::blink.getValue())
        drawBorder(0, 0, DISPLAY_LENGTH, DISPLAY_HEIGHT, 1, WHITE);
       if (state == PIEP && general::sound.getValue())
        digitalWrite(PIEZO, HIGH); //TODO: PIEP
    }
    if(state >= 3) {
      if(blinkSwitch == 20)
        blinkSwitch = 0;
      blinkSwitch++;
    } else if(blinkSwitch != 0)
      blinkSwitch = 1;
  }

  //Write PPM, Time
  void DisplayVX::writeInfo() {
    //ppm zeichnen
    if (lastState != state || start) {
      //Wenn sich der Wert geändert hat oder state sich geändert hat
      //Schreibt Status an die Decke
      dPrint(lastState.getTitle(), DISPLAY_LENGTH/2, STATUS_MARGIN_TOP, LAST_STATUS_SIZE, BACKGROUND_COLOR, 1);
      dPrint(state.getTitle(), DISPLAY_LENGTH/2, STATUS_MARGIN_TOP, STATUS_SIZE, state.getColor(COLORED_STATE), 1);

      //Draw Datatbox

      //Draw Bar
      if(COLORED_BAR || start) {
        display.fillRect(0, DATABOX_BAR_Y, DISPLAY_LENGTH, DATABOX_BAR_THICKNESS, state.getColor(COLORED_BAR));
        display.fillRect(0, TOP_BAR_HEIGHT, DISPLAY_LENGTH, TOP_BAR_THICKNESS, state.getColor(COLORED_BAR));
      }

      //Draw PPM
      if(airCondition < 0)
        airCondition = 0;
      dPrint(lastAirCondition, PPM_MARGIN_LEFT, PPM_Y, PPM_SIZE, DATABOX_BACKGROUND_COLOR, 6);
      dPrint(String(airCondition) + " ", PPM_MARGIN_LEFT, PPM_Y, PPM_SIZE, state.getColor(COLORED_PPM), 6);
      if (airCondition < 1000) {
        dPrint("ppm", PPM_STRING_X, PPM_STRING_Y, PPM_STRING_SIZE, state.getColor(COLORED_PPM), 6);
      }
    }

    if(state == VENTILATING) {
      short x = map(airCondition, Meassure::getLowest(), Meassure::getHighest(), 0, DISPLAY_LENGTH);
      display.fillRect(0, DATABOX_BAR_Y, x, DATABOX_BAR_THICKNESS, state.getColor(COLORED_BAR));
      display.fillRect(0, TOP_BAR_HEIGHT, x, TOP_BAR_THICKNESS, state.getColor(COLORED_BAR));

      display.fillRect(x+1, DATABOX_BAR_Y, DISPLAY_LENGTH, DATABOX_BAR_THICKNESS, GREY);
      display.fillRect(x+1, TOP_BAR_HEIGHT, DISPLAY_LENGTH, TOP_BAR_THICKNESS, GREY);

    }

    //Verhindert überschreiben von "ppm"
    if(airCondition >= 1000 && lastAirCondition < 1000)
      dPrint("ppm", PPM_STRING_X, PPM_STRING_Y, PPM_STRING_SIZE, DATABOX_BACKGROUND_COLOR, 6);

    //write new Pixels
    dPrint(airCondition, PPM_MARGIN_LEFT, PPM_Y, PPM_SIZE, state.getColor(COLORED_PPM), 6, DATABOX_BACKGROUND_COLOR, lastAirCondition);

    if (airCondition < 1000 && lastAirCondition >= 1000 || airCondition < 1000 && start)
      dPrint("ppm", PPM_STRING_X, PPM_STRING_Y, PPM_STRING_SIZE, state.getColor(COLORED_PPM), 6);
    //drawLoadingBar();

    //calculate time since last ventilating
    long startTime = Meassure::getStartTime();
    seconds = (millis() - startTime) / 1000 % 60;
    minutes = ((millis() - startTime) / 1000 - seconds) / 60;
    //create String
    time = "";
    if (minutes < 10)
      time = time + 0;
    time = time + minutes;
    time = time + ":";
    if (seconds < 10)
      time = time + 0;
    time = time + seconds;

    //Clear old Pixels
    //dPrint(lasttime, timeR_X, timeR_Y, timeR_SIZE, BAR_BACKGROUND_COLOR, 8);
    //write new Pixels
    if (minutes >= 20 && COLORED_TIME) {
      if(seconds == 0 && minutes == 20 || start) {
        dPrint(time, TIMER_X, TIMER_Y, TIMER_SIZE, TIME_COLOR_CRITICAL, 8, DATABOX_BACKGROUND_COLOR, start ? "" : lastTime);
        dPrint(":  ", TIMER_X, TIMER_Y, TIMER_SIZE, TIME_COLOR_CRITICAL, 8, DATABOX_BACKGROUND_COLOR, "   ");
      } else
        dPrint(time, TIMER_X, TIMER_Y, TIMER_SIZE, TIME_COLOR_CRITICAL, 8, DATABOX_BACKGROUND_COLOR, lastTime);
    } else {
      if(start)
        dPrint(time, TIMER_X, TIMER_Y, TIMER_SIZE, TIME_COLOR_NORMAL, 8);
      else
        dPrint(time, TIMER_X, TIMER_Y, TIMER_SIZE, TIME_COLOR_NORMAL, 8, DATABOX_BACKGROUND_COLOR, lastTime);
    }

    //Set new lasttime
    lastTime = time; //Setzt letzten Wert

  }
