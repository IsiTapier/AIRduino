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
short DisplayVX::peep = -1;
short DisplayVX::peepCounter = 0;

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

  void DisplayVX::handleData() {
    if(general::data.getValue())
      getData();
    else if(general::version.equals(V1))
      generateData(200, 1050, 5);
    else
      generateData(400, 1100, 100);
    //info
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
    
    if(general::theme.getValue()) {
      display.pushImage(MENU_ARROW_RESET_START_X, MENU_ARROW_RESET_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, arrowResetDark, BLACK);
      display.pushImage(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, homeDark, BLACK);
    } else {
      display.pushImage(MENU_ARROW_RESET_START_X, MENU_ARROW_RESET_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, arrowResetLight, WHITE);
      display.pushImage(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, homeLight, WHITE);
    }
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
    if(lastState == VENTILATING && state != VENTILATING)
      peep = -1;
    if(blinkSwitch) {
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
      if(peepCounter > 0)
        ledcDetachPin(PIEZO);
    } else {
      if(state >= 3 && general::blink.getValue()) {
        blinkSwitch = 1;
        drawBorder(0, 0, DISPLAY_LENGTH, DISPLAY_HEIGHT, 1, WHITE);
      }
      if(airCondition < PEEPSTART)
        return;
      short newPeep = floor((airCondition-PEEPSTART)/PEEPINTERVAL);
      if(PEEPEND != -1)
        newPeep = min((int)newPeep, PEEPEND);
      if(peep < newPeep) {
        peep = newPeep;
        if(general::sound.getValue()) {
          peepCounter = 1;
          debug(INFO, SENSOR, "Alarm - PEEP");
        }
      }
      if(peepCounter > 0) {
        if(peepCounter <= min(peep*INCREASEPEEPS+STARTPEEPS, MAXPEEPS)) {
          if(general::sound.getValue()) {
            ledcAttachPin(PIEZO, 0);
            ledcWriteNote(0, peep%2 == 0 ? NOTE_C : NOTE_G, min((int)floor(peep/2)+STARTOCTAVE, MAXOCTAVE));
          }
          peepCounter++;
          blinkSwitch = 1;
        } else {
          peepCounter = 0;
        }
      }
    }
  }

  //Write PPM, Time
  void DisplayVX::writeInfo() {
    if (lastState != state || start) {
      drawEssentials();
    }
    drawPPM();
    drawTimer();
    if(state == VENTILATING) {
      short x = map(airCondition, Meassure::getLowest(), Meassure::getHighest(), 0, DISPLAY_LENGTH);
      display.fillRect(0, DATABOX_BAR_Y, x, DATABOX_BAR_THICKNESS, state.getColor(COLORED_BAR));
      if(colorModes::showTopBar.getValue() && false)
        display.fillRect(0, TOP_BAR_HEIGHT, x, DATABOX_BAR_THICKNESS, state.getColor(COLORED_BAR));

      display.fillRect(x+1, DATABOX_BAR_Y, DISPLAY_LENGTH, DATABOX_BAR_THICKNESS, GREY);
      if(colorModes::showTopBar.getValue() && false)
        display.fillRect(x+1, TOP_BAR_HEIGHT, DISPLAY_LENGTH, DATABOX_BAR_THICKNESS, GREY);
    }
  }

  void DisplayVX::drawEssentials() {   
    //Status
    dPrint(lastState.getTitle(), DISPLAY_LENGTH/2, STATUS_MARGIN_TOP, LAST_STATUS_SIZE, BACKGROUND_COLOR, 1);
    dPrint(state.getTitle(), DISPLAY_LENGTH/2, STATUS_MARGIN_TOP, STATUS_SIZE, state.getColor(COLORED_STATE), 1);
    
    //Draw Bar
    if(COLORED_BAR || start) {
      display.fillRect(0, DATABOX_BAR_Y, DISPLAY_LENGTH, DATABOX_BAR_THICKNESS, state.getColor(COLORED_BAR));
      if(colorModes::showTopBar.getValue())
        display.fillRect(0, TOP_BAR_HEIGHT, DISPLAY_LENGTH, DATABOX_BAR_THICKNESS/* TOP_BAR_THICKNESS */, state.getColor(COLORED_BAR));
    }  
  }

  void DisplayVX::drawPPM() {
    String lastppm = getLastPPMString();   
    String ppm = getPPMString(lastppm);

    //Draw PPM
    dPrint(lastppm, PPM_MARGIN_LEFT, PPM_Y, ERRORLAST ? PPM_SIZE-1 : PPM_SIZE, DATABOX_BACKGROUND_COLOR, 6);
    dPrint(ppm, PPM_MARGIN_LEFT, PPM_Y, ERROR ? PPM_SIZE-1 : PPM_SIZE, ERROR ? RED : state.getColor(COLORED_PPM), 6);
    if (airCondition < 1000 && !ERROR) {
      dPrint("ppm", PPM_STRING_X, PPM_STRING_Y, PPM_STRING_SIZE, state.getColor(COLORED_PPM), 6);
    }

    //Verhindert überschreiben von "ppm"
    if((airCondition >= 1000 && lastAirCondition < 1000) || (ERROR && !ERRORLAST))
      dPrint("ppm", PPM_STRING_X, PPM_STRING_Y, PPM_STRING_SIZE, DATABOX_BACKGROUND_COLOR, 6);

    //write new Pixels
    dPrint(ppm, PPM_MARGIN_LEFT, PPM_Y, ERROR ? PPM_SIZE-1 : PPM_SIZE, ERROR ? RED : state.getColor(COLORED_PPM), 6, DATABOX_BACKGROUND_COLOR, lastppm, ERRORLAST ? PPM_SIZE-1 : PPM_SIZE); 
    if(airCondition < 1000 && !ERROR && (start || lastAirCondition >= 1000 || (ERRORLAST && !start)))
      dPrint("ppm", PPM_STRING_X, PPM_STRING_Y, PPM_STRING_SIZE, state.getColor(COLORED_PPM), 6);

  }

  void DisplayVX::drawTimer() {
    //calculate time since last ventilating
    unsigned long startTime = Meassure::getStartTime();
    seconds = (millis() - startTime) / 1000 % 60;
    minutes = ((millis() - startTime) / 1000 - seconds) / 60;

    if(minutes > 99)
      minutes = 99;
    
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
    dPrint(time, TIMER_X, TIMER_Y, TIMER_SIZE, TIME_COLOR, 8, DATABOX_BACKGROUND_COLOR, lastTime, -1, (seconds == 0 && (minutes == 0 || (minutes == 20 && COLORED_TIME))) || start);
    //Set new lasttime
    lastTime = time; //Setzt letzten Wert
  }

  String DisplayVX::getLastPPMString() {
    String lastppm;
    if(ERRORLAST)
      lastppm = "error";
    else if(lastAirCondition < 10)
      lastppm = "  " + String(lastAirCondition);
    else if(lastAirCondition < 100)
      lastppm = " " + String(lastAirCondition);
    else if(lastAirCondition > 9999)
      lastppm = "9999";
    else
      lastppm = String(lastAirCondition);
    return lastppm;
  }

  String DisplayVX::getPPMString(String lastppm) {
    String ppm;
    if(ERROR)
      ppm = "error";
    else if(airCondition <= 0)
      ppm = lastppm;
    else if(airCondition < 10)
      ppm = "  " + String(airCondition);
    else if(airCondition < 100)
      ppm = " " + String(airCondition);
    else if(airCondition > 9999)
      ppm = "9999";
    else
      ppm = String(airCondition);
    return ppm;
  }