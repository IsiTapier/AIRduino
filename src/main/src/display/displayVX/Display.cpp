/*
  Display.cpp - Display-Library for Librarys.
*/

#include "Arduino.h"
#include "Display.h"


  //   _____      _
  //  / ____|    | |
  // | (___   ___| |_ _   _ _ __
  //  \___ \ / _ \ __| | | | '_ \
  //  ____) |  __/ |_| |_| | |_) |
  // |_____/ \___|\__|\__,_| .__/
  //                       | |
  //                       |_|


  static void Display::setup() {
    pinMode(TFT_CS, OUTPUT);
    pinMode(TFT_RST, OUTPUT);
    pinMode(TFT_DC, OUTPUT);
    pinMode(TFT_LED, OUTPUT);
    pinMode(PIEZO, OUTPUT);

    analogWrite(TFT_LED, DISPLAY_BRIGHTNESS * 256);

    display.initR(INITR_BLACKTAB);
    display.setTextWrap(false);
    display.setRotation(ROTATION);

    loadingScreen();
  }

  //Loading Screen
  void Display::loadingScreen() {
    display.fillScreen(GRAPH_BACKGROUND_COLOR);
    writeLoadingScreenTitle();

    //Draw: Loading Dots
    byte c = 0;
    for (int x = LOADING_SCREEN_TIME * 2; x >= 0; x--) {
      c++;
      dPrint(55, 75, 3, GRAPH_BACKGROUND_COLOR, "..."); // Altes Clearen
      switch (c) {
        case 1: dPrint(55, 75, 3, LOADING_SCREEN_DOTS_COLOR, ".");
          break;
        case 2: dPrint(55, 75, 3, LOADING_SCREEN_DOTS_COLOR, "..");
          break;
        case 3: dPrint(55, 75, 3, LOADING_SCREEN_DOTS_COLOR, "...");
          c = 0;
          break;
      }
      delay(500);
    }
    drawDisplay();
  }

  static void Display::writeLoadingScreenTitle() {
    dPrint(50, 35, 4, LIGHT_BLUE, "A");
    dPrint(70, 35, 4, TURKISE, "I");
    dPrint(90, 35, 4, LIME, "R");
    dPrint(35, 65, 3, GREY, "duino");
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

  static void Display::drawDisplay() {
    display.fillScreen(GRAPH_BACKGROUND_COLOR);
    display.fillRect(0, DATABOX_TOP_HIGHT, DISPLAY_LENGTH + 1, DISPLAY_WIDTH, BAR_BACKGROUND_COLOR);
    createLines();
  }

  static void Display::checkState() {
    if (lastState) {
      drawBorder(0, 0, DISPLAY_LENGTH, DISPLAY_WIDTH, GRAPH_BACKGROUND_COLOR);
      display.fillRect(0, DATABOX_TOP_HIGHT, DISPLAY_LENGTH, DISPLAY_WIDTH, BAR_BACKGROUND_COLOR);
      createLines();
      digitalWrite(PIEZO, LOW);
      lastState = false;
    } else if (state >= 3) {
      drawBorder(0, 0, DISPLAY_LENGTH, DISPLAY_WIDTH, WHITE);
      if (state == PIEP) {
        //PIEP
        digitalWrite(PIEZO, HIGH);
      }
      lastState = true;
    }
    writeInfo();
  }

  //Write PPM, Time
  static void Display::writeInfo() {
    //ppm zeichnen
    if (lastAirConditionGraph != airCondition || previousState != state) {
      //Wenn sich der Wert geändert hat oder state sich geändert hat

      //Schreibt Status an die Decke
      switch (state) {
        case -1: statusInfo = "Lueftet..";
          break;
        case 0: statusInfo = "Optimal";
          break;
        case 1: statusInfo = "Schlecht";
          break;
        case 2: statusInfo = "Gammlig";
          break;
        default: statusInfo = "Gammlig";
          break;
      }
      display.fillRect(30, 0, 110, 15, GRAPH_BACKGROUND_COLOR);
      dPrint(30, 0, 2, WHITE, statusInfo);

      //Verhindert überschreiben von "ppm"
      if (airCondition < 1000)
        dPrint(57, 118, 1.5, Util::getColor(state), "ppm");
      else
        dPrint(57, 118, 1.5, BAR_BACKGROUND_COLOR, "ppm");

      //Clear old Pixels
      dPrint(2, 105, 3, BAR_BACKGROUND_COLOR, lastAirConditionGraph);
      //write new Pixels
      dPrint(2, 105, 3, Util::getColor(state), airCondition);
      //Set new lastAirCondition
      lastAirConditionGraph = airCondition; //Setzt letzten Wert
      //Draw Bar
      display.fillRect(0, DATABOX_TOP_HIGHT, DISPLAY_LENGTH + 1, BAR_STRIPE_THICKNESS, Util::getColor(state));
      previousState = state;
    }
    //Draw Loading Bar
    if (state == VENTILATING) {
      short bar_ventilating_length = map(airCondition, Meassure::getLowest(), Meassure::getPpmSinceVentilation(), 0, DISPLAY_LENGTH);
      display.fillRect(DISPLAY_LENGTH - bar_ventilating_length, DATABOX_TOP_HIGHT, bar_ventilating_length, BAR_STRIPE_THICKNESS, GREY);
    }


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
    dPrint(96, 109, 2, BAR_BACKGROUND_COLOR, lastTime);
    //write new Pixels
    if (minutes >= 20)
      dPrint(96, 109, 2, TIME_COLOR_CRITICAL, Time);
    else
      dPrint(96, 109, 2, TIME_COLOR_NORMAL, Time);
    //Set new lasttime
    lastTime = Time; //Setzt letzten Wert
  }

  static void Display::drawBorder(int xStart, int yStart, int xEnd, int yEnd, int color) {
    display.drawLine(xStart, yStart, xEnd, yStart, color);
    display.drawLine(xStart, yEnd, xEnd, yEnd, color);
    display.drawLine(xStart, yStart, xStart, yEnd, color);
    display.drawLine(xEnd, yStart, xEnd, yEnd, color);
  }

  static void Display::drawLine(int x, int y, int z) {
    for (int i = 0; i < x; i = i + z) {
      display.drawPixel(i, y, WHITE);
    }
  }

  static void Display::createLines() {}

  static void Display::dPrint(int x, int y, int scale, int color, String text) {
    display.setCursor(x, y);
    display.setTextSize(scale);
    display.setTextColor(color);
    display.println(text);
  }

  //Verkürzung: Writing mit Integern
  static void Display::dPrint(int x, int y, int scale, int color, int text) {
    display.setCursor(x, y);
    display.setTextSize(scale);
    display.setTextColor(color);
    display.println(text);
  }


  static Adafruit_ST7735 Display::display = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
  static State Display::state;
  static State Display::previousState;
  static int Display::airCondition;
  static int Display::lastAirConditionGraph;
  static int Display::lastState = false;
  static String Display::statusInfo;
  static String Display::lastTime;
  static String Display::Time;
  static short Display::seconds;
  static short Display::minutes;
