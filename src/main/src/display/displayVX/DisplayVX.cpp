/*
  DisplayVX.cpp - DisplayVX-Library for Librarys.
*/

#include "Arduino.h"
#include "DisplayVX.h"

State DisplayVX::state;
State DisplayVX::lastState;
int DisplayVX::airCondition;
int DisplayVX::lastAirCondition = 0;
boolean DisplayVX::blinkSwitch = 0;
int DisplayVX::statusLetters;
String DisplayVX::statusInfo;
String DisplayVX::lastTime;
String DisplayVX::time;
short DisplayVX::seconds;
short DisplayVX::minutes;
boolean DisplayVX::start;
boolean DisplayVX::drop = false;
boolean DisplayVX::lastError = false;

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

  void DisplayVX::setup() {
    debug(DEBUG, SETUP, "DisplayVX SETUP started");
    start = true;
    debug(DEBUG, SETUP, "DisplayVX SETUP completed");
    debug(DEBUG, SETUP, "");
  }

  void DisplayVX::loop(boolean draw) {
    if(general::data.getValue())
      getData();
    else if(general::version.equals(V1))
      generateData(200, 1050, 5);
    else
      generateData(400, 1100, 100);
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
    if(general::theme.getValue())
      display.pushImage(MENU_ARROW_RESET_START_X, MENU_ARROW_RESET_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, arrowResetDark, BLACK);
    else
      display.pushImage(MENU_ARROW_RESET_START_X, MENU_ARROW_RESET_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, arrowResetLight, WHITE);
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
    if (blinkSwitch) {
      blinkSwitch = 0;
      //restore Border
      drawBorder(0, 0, DISPLAY_LENGTH, DISPLAY_HEIGHT, 1, BACKGROUND_COLOR);
      display.drawLine(0, DISPLAY_HEIGHT-1, DISPLAY_LENGTH-1, DISPLAY_HEIGHT-1, DATABOX_BACKGROUND_COLOR);
      display.drawLine(0, DATABOX_Y, 0, DISPLAY_HEIGHT-1, DATABOX_BACKGROUND_COLOR);
      display.drawLine(DISPLAY_LENGTH-1, DATABOX_Y, DISPLAY_LENGTH-1, DISPLAY_HEIGHT-1, DATABOX_BACKGROUND_COLOR);
      display.drawLine(0, DATABOX_BAR_Y, 0, DATABOX_Y-1, state.getColor(COLORED_BAR));
      display.drawLine(DISPLAY_LENGTH-1, DATABOX_BAR_Y, DISPLAY_LENGTH-1, DATABOX_Y-1, state.getColor(COLORED_BAR));
      if(colorModes::showTopBar.getValue()) {
        display.drawLine(0, TOP_BAR_HEIGHT, 0, TOP_BAR_HEIGHT+DATABOX_BAR_THICKNESS-1, state.getColor(COLORED_BAR));
        display.drawLine(DISPLAY_LENGTH-1, TOP_BAR_HEIGHT, DISPLAY_LENGTH-1, TOP_BAR_HEIGHT+DATABOX_BAR_THICKNESS-1, state.getColor(COLORED_BAR));
      }
      ledcDetachPin(PIEZO);
    } else if(state >= 3) {
      blinkSwitch = 1;
      if(general::blink.getValue())
        drawBorder(0, 0, DISPLAY_LENGTH, DISPLAY_HEIGHT, 1, WHITE);
       if (state >= PIEP && general::sound.getValue()) {
        ledcAttachPin(PIEZO, 0);
        ledcWriteNote(0, NOTE_C, 4);
        debug(SPAMM, SENSOR, "Alarm - PEEP");
       }
    }
  }

  //Write PPM, Time
  void DisplayVX::writeInfo() {
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
      //Wenn sich der Wert geändert hat oder state sich geändert hat
      //Schreibt Status an die Decke
      dPrint(lastState.getTitle(), DISPLAY_LENGTH/2, STATUS_MARGIN_TOP, LAST_STATUS_SIZE, BACKGROUND_COLOR, 1);
      dPrint(state.getTitle(), DISPLAY_LENGTH/2, STATUS_MARGIN_TOP, STATUS_SIZE, state.getColor(COLORED_STATE), 1);

      //Draw Datatbox

      //Draw Bar
      // if((BLACK_MODE == 0) && (general::version.getValue() == 0)) {
        if(COLORED_BAR || start) {
          display.fillRect(0, DATABOX_BAR_Y, DISPLAY_LENGTH, DATABOX_BAR_THICKNESS, state.getColor(COLORED_BAR));
          if(colorModes::showTopBar.getValue())
            display.fillRect(0, TOP_BAR_HEIGHT, DISPLAY_LENGTH, DATABOX_BAR_THICKNESS/* TOP_BAR_THICKNESS */, state.getColor(COLORED_BAR));
        }
      // }
      //Draw PPM
      dPrint(lastppm, PPM_MARGIN_LEFT, PPM_Y, ERRORLAST ? PPM_SIZE-1 : PPM_SIZE, DATABOX_BACKGROUND_COLOR, 6);
      dPrint(ppm, PPM_MARGIN_LEFT, PPM_Y, ERROR ? PPM_SIZE-1 : PPM_SIZE, ERROR ? RED : state.getColor(COLORED_PPM), 6);
      if (airCondition < 1000 && (airCondition > 0 || (!ERRORLAST && lastAirCondition < 1000))) {
        dPrint("ppm", PPM_STRING_X, PPM_STRING_Y, PPM_STRING_SIZE, state.getColor(COLORED_PPM), 6);
      }
    }

    if(state == VENTILATING) {
      short x = map(airCondition, Meassure::getLowest(), Meassure::getHighest(), 0, DISPLAY_LENGTH);
      display.fillRect(0, DATABOX_BAR_Y, x, DATABOX_BAR_THICKNESS, state.getColor(COLORED_BAR));
      if(colorModes::showTopBar.getValue() && false)
        display.fillRect(0, TOP_BAR_HEIGHT, x, DATABOX_BAR_THICKNESS, state.getColor(COLORED_BAR));

      display.fillRect(x+1, DATABOX_BAR_Y, DISPLAY_LENGTH, DATABOX_BAR_THICKNESS, GREY);
      if(colorModes::showTopBar.getValue() && false)
        display.fillRect(x+1, TOP_BAR_HEIGHT, DISPLAY_LENGTH, DATABOX_BAR_THICKNESS, GREY);
    }

    //Verhindert überschreiben von "ppm"
    if((airCondition >= 1000 && lastAirCondition < 1000) || (ERROR && !lastError))
      dPrint("ppm", PPM_STRING_X, PPM_STRING_Y, PPM_STRING_SIZE, DATABOX_BACKGROUND_COLOR, 6);

    //write new Pixels
    dPrint(ppm, PPM_MARGIN_LEFT, PPM_Y, ERROR ? PPM_SIZE-1 : PPM_SIZE, ERROR ? RED : state.getColor(COLORED_PPM), 6, DATABOX_BACKGROUND_COLOR, lastppm, ERRORLAST ? PPM_SIZE-1 : PPM_SIZE);

    if ((airCondition < 1000 && lastAirCondition >= 1000 && !ERROR) || (airCondition < 1000 && start && !ERROR) || (!ERROR && ERRORLAST && airCondition < 1000)) 
      dPrint("ppm", PPM_STRING_X, PPM_STRING_Y, PPM_STRING_SIZE, state.getColor(COLORED_PPM), 6);
    //drawLoadingBar();)

    //calculate time since last ventilating
    unsigned long startTime = Meassure::getStartTime();
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
    //write new Pixels
    dPrint(time, TIMER_X, TIMER_Y, TIMER_SIZE, minutes>=20&&COLORED_TIME?TIME_COLOR_CRITICAL:TIME_COLOR_NORMAL, 8, DATABOX_BACKGROUND_COLOR, lastTime, -1, (seconds == 0 && (minutes == 0 || (minutes == 20 && COLORED_TIME))) || start);
    //Set new lasttime
    lastTime = time; //Setzt letzten Wert
    lastError = ERRORLAST;
  }
