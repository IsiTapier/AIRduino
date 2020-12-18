/*
  Display.cpp - Display-Library for Librarys.
*/

#include "Arduino.h"
#include "Display.h"

static Adafruit_TFTLCD Display::display(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
static TouchScreen Display::ts = TouchScreen(XP, YP, XM, YM, 300);
static uint16_t Display::identifier = display.readID();

static State Display::state;
static State Display::previousState;
static int Display::airCondition;
static int Display::lastAirConditionGraph;
static boolean Display::blinkSwitch = false;
static int Display::statusLetters;
static String Display::statusInfo;
static String Display::lastTime;
static String Display::Time;
static short Display::seconds;
static short Display::minutes;




  //   _____      _
  //  / ____|    | |
  // | (___   ___| |_ _   _ _ __
  //  \___ \ / _ \ __| | | | '_ \
  //  ____) |  __/ |_| |_| | |_) |
  // |_____/ \___|\__|\__,_| .__/
  //                       | |
  //                       |_|


  static void Display::setup() {
    pinMode(PIEZO, OUTPUT);
    //TODO Helligkeit einstellen

    if(identifier == 0x9325) {
      Serial.println(F("Found ILI9325 LCD driver"));
    } else if(identifier == 0x9328) {
      Serial.println(F("Found ILI9328 LCD driver"));
    } else if(identifier == 0x7575) {
      Serial.println(F("Found HX8347G LCD driver"));
    } else if(identifier == 0x9341) {
      Serial.println(F("Found ILI9341 LCD driver"));
    } else if(identifier == 0x8357) {
      Serial.println(F("Found HX8357D LCD driver"));
    } else {
      Serial.print(F("Unknown LCD driver chip: "));
      Serial.println(identifier, HEX);
      Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
      Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
      Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
      Serial.println(F("If using the breakout board, it should NOT be #defined!"));
      Serial.println(F("Also if using the breakout, double-check that all wiring"));
      Serial.println(F("matches the tutorial."));
      return;
    }


    display.begin(identifier);

    display.fillScreen(GRAPH_BACKGROUND_COLOR);
    display.setTextWrap(false);
    display.setRotation(ROTATION);

    loadingScreen();
  }

  //Loading Screen
  void Display::loadingScreen() {
    Serial.println("test1");
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
    Serial.println("test2");
    drawDisplay();
  }

  static void Display::writeLoadingScreenTitle() {
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

  static void Display::drawDisplay() {
    display.fillScreen(GRAPH_BACKGROUND_COLOR);
    createLines();
  }

  static void Display::checkState() {
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
  static void Display::writeInfo() {
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
  static void Display::dPrint(int x, int y, int scale, int color, int text) {
    display.setCursor(x, y);
    display.setTextSize(scale);
    display.setTextColor(color);
    display.println(text);
  }

  static void Display::drawLoadingBar() {

  }
