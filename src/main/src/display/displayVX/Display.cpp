/*
  Display.cpp - Display-Library for Librarys.
*/

#include "Arduino.h"
#include "Display.h"

extern TFT_eSPI Display::display(240, 320);
//extern TouchScreen Display::ts = TouchScreen(XP, YP, XM, YM, 300);

extern State Display::state;
extern State Display::previousState;
extern int Display::airCondition;
extern int Display::lastAirConditionGraph;
extern boolean Display::blinkSwitch = false;
extern int Display::statusLetters;
extern String Display::statusInfo;
extern String Display::lastTime;
extern String Display::Time;
extern short Display::seconds;
extern short Display::minutes;




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
    display.fillScreen(GRAPH_BACKGROUND_COLOR);
    display.setTextWrap(false);
    display.setRotation(ROTATION);
    Serial.println("Display initialized");
    loadingScreen();
    Serial.println("DISPLAY SETUP complete");
    Serial.println();
  }

  //Loading Screen
  void Display::loadingScreen() {
    Serial.println("loadingscreen started");
    display.fillScreen(GRAPH_BACKGROUND_COLOR);
    writeLoadingScreenTitle();

    //Draw: Loading Dots
    short distanceToFirstDot = (DISPLAY_LENGTH - 15*LOADING_SCREEN_DOT_SIZE) / 2;
    byte c = 0;
    for (int x = LOADING_SCREEN_TIME * 2; x >= 0; x--) {
      c++;
      dPrint(distanceToFirstDot, DISPLAY_WIDTH / 5 * 3, LOADING_SCREEN_DOT_SIZE, GRAPH_BACKGROUND_COLOR, "..."); // Altes Clearen
      switch (c) {
        case 1: dPrint(distanceToFirstDot, DISPLAY_WIDTH / 5 * 3, LOADING_SCREEN_DOT_SIZE, LOADING_SCREEN_DOTS_COLOR, ".");
          break;
        case 2: dPrint(distanceToFirstDot, DISPLAY_WIDTH / 5 * 3, LOADING_SCREEN_DOT_SIZE, LOADING_SCREEN_DOTS_COLOR, "..");
          break;
        case 3: dPrint(distanceToFirstDot, DISPLAY_WIDTH / 5 * 3, LOADING_SCREEN_DOT_SIZE, LOADING_SCREEN_DOTS_COLOR, "...");
          c = 0;
          break;
      }
      delay(500);
    }
    Serial.println("loadingscreen ended");
    drawDisplay();
    Serial.println("Display drawn");
  }

  extern void Display::writeLoadingScreenTitle() {
    short distanceToFirstLetter = (DISPLAY_LENGTH - (18*LOADING_SCREEN_TITLE_SIZE)) / 2;
    short distanceToFirstLetterSub = (DISPLAY_LENGTH - (30*LOADING_SCREEN_SUB_SIZE)) / 2;
    dPrint(distanceToFirstLetter + LOADING_SCREEN_TITLE_SIZE, 35, LOADING_SCREEN_TITLE_SIZE, LIGHT_BLUE, "A");
    dPrint(distanceToFirstLetter + 6*LOADING_SCREEN_TITLE_SIZE, 35, LOADING_SCREEN_TITLE_SIZE, TURKISE, "I");
    dPrint(distanceToFirstLetter + 12*LOADING_SCREEN_TITLE_SIZE, 35, LOADING_SCREEN_TITLE_SIZE, LIME, "R");
    dPrint(distanceToFirstLetterSub, 120, LOADING_SCREEN_SUB_SIZE, GREY, "duino");
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
    display.fillScreen(GRAPH_BACKGROUND_COLOR);
    createLines();
  }

  extern void Display::checkState() {
    if (blinkSwitch) {
      drawBorder(0, 0, DISPLAY_LENGTH, DISPLAY_WIDTH, GRAPH_BACKGROUND_COLOR);
      display.fillRect(0, DATABOX_TOP_HIGHT, DISPLAY_LENGTH, DISPLAY_WIDTH, BAR_BACKGROUND_COLOR);
      createLines();
      digitalWrite(PIEZO, LOW);
      blinkSwitch = false;
    } else if (state >= 3) {
      drawBorder(0, 0, DISPLAY_LENGTH, DISPLAY_WIDTH, WHITE);
      if (state == PIEP) {
        //PIEP
        digitalWrite(PIEZO, HIGH);
      }
      blinkSwitch = true;
    }
    writeInfo();
  }

  //Write PPM, Time
  extern void Display::writeInfo() {
    //ppm zeichnen
    if (lastAirConditionGraph != airCondition || previousState != state) {
      //Wenn sich der Wert geändert hat oder state sich geändert hat

      //Schreibt Status an die Decke
      dPrint((DISPLAY_LENGTH - (6*statusLetters-1)*2)/2, STATUS_INFO_TOP_MARGIN, 2, GRAPH_BACKGROUND_COLOR, statusInfo);
      switch (state) {
        case -1: statusInfo = "Lueftet";
                 statusLetters = 6;
          break;
        case 0:  statusInfo = "Optimal";
                 statusLetters = 7;
          break;
        case 1:  statusInfo = "Schlecht";
                 statusLetters = 8;
          break;
        case 2:  statusInfo = "Gammlig";
                 statusLetters = 7;
          break;
        case 3:  statusInfo = "sehr Gammlig";
                 statusLetters = 12;
          break;
        case 4:  statusInfo = "ueber Gammlig";
                 statusLetters = 12;
          break;
        default: statusInfo = "Undefiniert";
                 statusLetters = 11;
          break;
      }
      dPrint((DISPLAY_LENGTH - (6*statusLetters-1)*2)/2, STATUS_INFO_TOP_MARGIN, 2, WHITE, statusInfo);

      //Verhindert überschreiben von "ppm"
      short PPM_DIGIT_Y = DISPLAY_WIDTH - PPM_DIGIT_SIZE * 10;

      if (airCondition < 1000)
        dPrint(PPM_DIGIT_MARGIN + 3*6*PPM_DIGIT_SIZE, PPM_DIGIT_Y, PPM_STRING_SIZE, Util::getColor(state), "ppm");
      else
        dPrint(PPM_DIGIT_MARGIN + 3*6*PPM_DIGIT_SIZE, PPM_DIGIT_Y, PPM_STRING_SIZE, BAR_BACKGROUND_COLOR, "ppm");

      //Clear old Pixels

      dPrint(PPM_DIGIT_MARGIN, PPM_DIGIT_Y, PPM_DIGIT_SIZE, BAR_BACKGROUND_COLOR, lastAirConditionGraph);
      //write new Pixels
      dPrint(PPM_DIGIT_MARGIN, PPM_DIGIT_Y, PPM_DIGIT_SIZE, Util::getColor(state), airCondition) ;
      //Set new lastAirCondition
      lastAirConditionGraph = airCondition; //Setzt letzten Wert
      //Draw Bar

      previousState = state;
    }
    drawLoadingBar();

    //calculate time since last ventilating
    long startTime = Meassure::getStartTime();
    seconds = (millis() - startTime) / 1000 % 60;
    minutes = ((millis() - startTime) / 1000 - seconds) / 60;

    //create String
    Time = "";
    if (minutes < 10)
      Time = Time + 0;
    Time = Time + minutes;
    Time = Time + ":";
    if (seconds < 10)
      Time = Time + 0;
    Time = Time + seconds;

    //Clear old Pixels
    dPrint(180, 180, TIMER_SIZE, BAR_BACKGROUND_COLOR, lastTime);
    //write new Pixels
    if (minutes >= 20)
      dPrint(180, 180, TIMER_SIZE, TIME_COLOR_CRITICAL, Time);
    else
      dPrint(180, 180, TIMER_SIZE, TIME_COLOR_NORMAL, Time);
    //Set new lasttime
    lastTime = Time; //Setzt letzten Wert
  }

  extern void Display::drawBorder(int xStart, int yStart, int xEnd, int yEnd, int color) {
    display.drawLine(xStart, yStart, xEnd, yStart, color);
    display.drawLine(xStart, yEnd, xEnd, yEnd, color);
    display.drawLine(xStart, yStart, xStart, yEnd, color);
    display.drawLine(xEnd, yStart, xEnd, yEnd, color);
  }


  extern void Display::drawLine(int x, int y, int z) {
    for (int i = 0; i < x; i = i + z) {
      display.drawPixel(i, y, WHITE);
    }
  }

  extern void Display::createLines() {}

  extern void Display::dPrint(int x, int y, int scale, int color, String text) {
    display.setCursor(x, y);
    display.setTextSize(scale);
    display.setTextColor(color);
    for(int i = 0; i < text.length(); i++) {
      if(text.charAt(i) == (char)'u' && text.charAt(i+1) == (char) 'e') {
        display.print(char(0x81));
        i++;
      } else
        display.print(text.charAt(i));
    }
    display.println();
  }

  //Verkürzung: Writing mit Integern
  extern void Display::dPrint(int x, int y, int scale, int color, int text) {
    display.setCursor(x, y);
    display.setTextSize(scale);
    display.setTextColor(color);
    display.println(text);
  }

  extern void Display::drawLoadingBar() {

  }
