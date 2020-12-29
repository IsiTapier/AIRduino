/*
  Display.cpp - Display-Library for Librarys.
*/

#include "Arduino.h"
#include "Display.h"

extern TFT_eSPI Display::display(DISPLAY_HEIGHT, DISPLAY_LENGTH);
//extern TouchScreen Display::ts = TouchScreen(XP, YP, XM, YM, 300);
extern State Display::state;
extern State Display::lastState;
extern int Display::airCondition;
extern int Display::lastAirCondition = 0;
extern boolean Display::blinkSwitch = false;
extern int Display::statusLetters;
extern String Display::statusInfo;
extern String Display::lastTime;
extern String Display::time;
extern short Display::seconds;
extern short Display::minutes;
extern boolean Display::start;
extern boolean Display::drop = false;




  //   _____      _
  //  / ____|    | |
  // | (___   ___| |_ _   _ _ __
  //  \___ \ / _ \ __| | | | '_ \
  //  ____) |  __/ |_| |_| | |_) |
  // |_____/ \___|\__|\__,_| .__/
  //                       | |
  //                       |_|


  extern void Display::setup() {
    Serial.println("DISPLAY SETUP started");
    display.begin();
    Serial.println("Display connection started");
    display.fillScreen(BACKGROUND_COLOR);
    display.setTextWrap(false);
    display.setRotation(ROTATION);
    Serial.println("Display initialized");
    loadingScreen();
    start = true;
    Serial.println("DISPLAY SETUP complete");
    Serial.println();
  }

  //Loading Screen
  void Display::loadingScreen() {
    Serial.println("loadingscreen started");
    display.fillScreen(BACKGROUND_COLOR);
    writeLoadingScreenTitle();

    //Draw: Loading Dots
    short distanceToFirstDot = (DISPLAY_LENGTH - 15*LOADING_SCREEN_DOT_SIZE) / 2;
    byte c = 0;
    for (int x = LOADING_SCREEN_TIME * 2; x >= 0; x--) {
      c++;
      dPrint("...", distanceToFirstDot, DISPLAY_HEIGHT / 5 * 3, LOADING_SCREEN_DOT_SIZE, BACKGROUND_COLOR); // Altes Clearen
      switch (c) {
        case 1: dPrint(".", distanceToFirstDot, DISPLAY_HEIGHT / 5 * 3, LOADING_SCREEN_DOT_SIZE, LOADING_SCREEN_DOTS_COLOR);
          break;
        case 2: dPrint("..", distanceToFirstDot, DISPLAY_HEIGHT / 5 * 3, LOADING_SCREEN_DOT_SIZE, LOADING_SCREEN_DOTS_COLOR);
          break;
        case 3: dPrint("...", distanceToFirstDot, DISPLAY_HEIGHT / 5 * 3, LOADING_SCREEN_DOT_SIZE, LOADING_SCREEN_DOTS_COLOR);
          c = 0;
          break;
      }
      delay(500);
    }
    Serial.println("loadingscreen ended");
  }

  extern void Display::writeLoadingScreenTitle() {
    short distanceToFirstLetter = (DISPLAY_LENGTH - (18*LOADING_SCREEN_TITLE_SIZE)) / 2;
    short distanceToFirstLetterSub = (DISPLAY_LENGTH - (30*LOADING_SCREEN_SUB_SIZE)) / 2;
    dPrint("A", distanceToFirstLetter + LOADING_SCREEN_TITLE_SIZE, 35, LOADING_SCREEN_TITLE_SIZE, LIGHT_BLUE);
    dPrint("I", distanceToFirstLetter + 6*LOADING_SCREEN_TITLE_SIZE, 35, LOADING_SCREEN_TITLE_SIZE, TURKISE);
    dPrint("R", distanceToFirstLetter + 12*LOADING_SCREEN_TITLE_SIZE, 35, LOADING_SCREEN_TITLE_SIZE, LIME);
    dPrint("duino", distanceToFirstLetterSub, 120, LOADING_SCREEN_SUB_SIZE, GREY);
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

  extern void Display::drawDisplay() {
    display.fillScreen(BACKGROUND_COLOR);
    display.fillRect(0, DATABOX_Y, DISPLAY_LENGTH, DATABOX_HEIGHT, DATABOX_BACKGROUND_COLOR);
    createLines();
    Serial.println("Display drawn");
  }

  extern void Display::getData() {
    state = Meassure::getState();
    airCondition = Meassure::getAirCondition();
  }

  extern void Display::generateData(int startPPM, int endPPM, int changePPM) {
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
    state = Util::getStateOf(airCondition);
  }

  extern void Display::checkState() {
    if (blinkSwitch) {
      drawBorder(0, 0, DISPLAY_LENGTH, DISPLAY_HEIGHT, 1, BACKGROUND_COLOR);
      display.drawLine(0, DISPLAY_HEIGHT, DISPLAY_LENGTH, DISPLAY_HEIGHT, DATABOX_BACKGROUND_COLOR);
      display.drawLine(0, DATABOX_BAR_Y, 0, DATABOX_HEIGHT, DATABOX_BACKGROUND_COLOR);
      display.drawLine(DISPLAY_LENGTH, DATABOX_BAR_Y, DISPLAY_LENGTH, DATABOX_HEIGHT, DATABOX_BACKGROUND_COLOR);
      display.fillRect(0, DATABOX_BAR_Y, DISPLAY_LENGTH, DATABOX_BAR_THICKNESS, state.getColor(COLORED_BAR));
      //createLines();
      digitalWrite(PIEZO, LOW);
      blinkSwitch = false;
    } else if (state >= 3) {
      drawBorder(0, 0, DISPLAY_LENGTH, DISPLAY_HEIGHT, 1, WHITE);
      if (state == PIEP)
        digitalWrite(PIEZO, HIGH); //TODO: PIEP

      blinkSwitch = true;
    }
  }

  //Write PPM, Time
  extern void Display::writeInfo() {
    //ppm zeichnen
    if (lastState != state || start) {
      //Wenn sich der Wert geändert hat oder state sich geändert hat
      //Schreibt Status an die Decke
      dPrint(lastState.getTitle(), DISPLAY_LENGTH/2, STATUS_MARGIN_TOP, LAST_STATUS_SIZE, BACKGROUND_COLOR, 1);
      dPrint(state.getTitle(), DISPLAY_LENGTH/2, STATUS_MARGIN_TOP, STATUS_SIZE, state.getColor(COLORED_STATE), 1);

      //Draw Datatbox

      //Draw Bar
      if(COLORED_BAR || start)
        display.fillRect(0, DATABOX_BAR_Y, DISPLAY_LENGTH, DATABOX_BAR_THICKNESS, state.getColor(COLORED_BAR));

      //Draw PPM
      dPrint(lastAirCondition, PPM_MARGIN_LEFT, PPM_Y, PPM_SIZE, DATABOX_BACKGROUND_COLOR, 6);
      dPrint(String(airCondition) + " ", PPM_MARGIN_LEFT, PPM_Y, PPM_SIZE, state.getColor(COLORED_PPM), 6);
      if (airCondition < 1000) {
        dPrint("ppm", PPM_STRING_X, PPM_STRING_Y, PPM_STRING_SIZE, state.getColor(COLORED_PPM), 6);
      }
    }

    lastState = state;


    //write new Pixels
    dPrint(airCondition, PPM_MARGIN_LEFT, PPM_Y, PPM_SIZE, state.getColor(COLORED_PPM), 6, DATABOX_BACKGROUND_COLOR, lastAirCondition);

    //Verhindert überschreiben von "ppm"
    if (airCondition < 1000 && lastAirCondition >= 1000 || airCondition < 1000 && start) {
      dPrint(String(airCondition) + " ", PPM_MARGIN_LEFT, PPM_Y, PPM_SIZE, state.getColor(COLORED_PPM), 6, DATABOX_BACKGROUND_COLOR, String(lastAirCondition));
      dPrint("ppm", PPM_STRING_X, PPM_STRING_Y, PPM_STRING_SIZE, state.getColor(COLORED_PPM), 6);
    } else if(airCondition >= 1000 && lastAirCondition < 1000)
      dPrint("ppm", PPM_STRING_X, PPM_STRING_Y, PPM_STRING_SIZE, DATABOX_BACKGROUND_COLOR, 6);


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
      if(seconds == 0 && minutes == 20)
        dPrint(time, TIMER_X, TIMER_Y, TIMER_SIZE, TIME_COLOR_CRITICAL, 8, DATABOX_BACKGROUND_COLOR);
      else
        dPrint(time, TIMER_X, TIMER_Y, TIMER_SIZE, TIME_COLOR_CRITICAL, 8, DATABOX_BACKGROUND_COLOR, lastTime);
    } else
      dPrint(time, TIMER_X, TIMER_Y, TIMER_SIZE, TIME_COLOR_NORMAL, 8, DATABOX_BACKGROUND_COLOR, lastTime);
    //Set new lasttime
    lastTime = time; //Setzt letzten Wert


    lastAirCondition = airCondition;

  }

  extern void Display::drawBorder(int x, int y, int length, int height, int thickness, int color) {
    length -= thickness;
    height -= thickness;
    for(int i = 0; i < thickness; i++) {
      for(int i = 0; i < thickness; i++) {
        display.drawLine(x, y, x+length, y, color);
        display.drawLine(x, y+height, x+length, y+height, color);
        display.drawLine(x, y, x, y+height, color);
        display.drawLine(x+length, y, x+length, y+height, color);
        x++;
      }
      y++;
      x -= thickness;
    }
  }


  extern void Display::drawLine(int x, int y, int z, boolean direction) {
    if(direction) {
      for (int i = 0; i < x; i = i + z) {
        display.drawPixel(i, y, WHITE);
      }
    } else {
      for (int i = 0; i < y; i = i + z) {
        display.drawPixel(x, i, WHITE);
      }
    }
  }

  extern void Display::createLines() {}

  extern void Display::showBoxes() {
    display.fillRect(PPM_MARGIN_LEFT, DISPLAY_HEIGHT - PPM_HEIGHT, PPM_LENGTH, PPM_HEIGHT, GREEN);
    display.fillRect(DISPLAY_LENGTH - TIMER_LENGTH - TIMER_MARGIN_RIGHT, DISPLAY_HEIGHT - TIMER_MARGIN_BOTTOM - TIMER_HEIGHT, TIMER_LENGTH, TIMER_HEIGHT, WHITE);
    display.fillRect(DISPLAY_LENGTH - TIMER_LENGTH - TIMER_MARGIN_RIGHT, DISPLAY_HEIGHT - TIMER_MARGIN_BOTTOM - TIMER_SIZE*LETTER_HEIGHT, TIMER_LENGTH, TIMER_SIZE*LETTER_HEIGHT, BLUE);
    display.fillRect((DISPLAY_LENGTH - STATUS_LENGTH)/2, STATUS_MARGIN_TOP, STATUS_LENGTH, STATUS_HEIGHT, RED);
  }

  extern void Display::dPrint(String text, int x, int y, int scale, int color, int datum, int backgroundColor, String oldText, int padding) {
    display.setTextSize(scale);
    display.setTextPadding(padding);
    display.setTextDatum(datum);
    if(backgroundColor == 0)
      backgroundColor = BACKGROUND_COLOR;
    if(oldText.length() != 0) {
      if(datum % 3 == 2)
        x -= LETTER_LENGTH*scale*(text.length()-1);
      for(int i = 0; i < text.length(); i++) {
        if(text.charAt(i) != oldText.charAt(i)) {
          display.setTextColor(backgroundColor);
          display.drawString((String) oldText.charAt(i), x+LETTER_LENGTH*scale*i, y);
          display.setTextColor(color);
          display.drawString((String) text.charAt(i), x+LETTER_LENGTH*scale*i, y);
        }
      }
    } else {
      display.setCursor(x, y);
      if(backgroundColor > 0)
        display.setTextColor(color, backgroundColor);
      else
        display.setTextColor(color);
      display.drawString(text);
    }
  }

  //Verkürzung: Writing mit Integern
  extern void Display::dPrint(int text, int x, int y, int scale, int color, int datum, int backgroundColor, int oldText, int padding) {
    dPrint(String(text), x, y, scale, color, datum, backgroundColor, (oldText == -1) ? "" : String(oldText), padding);
  }

  extern void Display::drawLoadingBar() {

  }
