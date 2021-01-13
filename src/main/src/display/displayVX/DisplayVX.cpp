/*
  DisplayVX.cpp - DisplayVX-Library for Librarys.
*/

#include "Arduino.h"
#include "DisplayVX.h"

extern TFT_eSPI DisplayVX::display(DISPLAY_HEIGHT, DISPLAY_LENGTH);
extern TouchScreen DisplayVX::ts;
extern State DisplayVX::state;
extern State DisplayVX::lastState;
extern int DisplayVX::airCondition;
extern int DisplayVX::lastAirCondition = 0;
extern boolean DisplayVX::blinkSwitch = false;
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


  extern void DisplayVX::setup() {
    Serial.println("DISPLAY SETUP started");
    display.begin();
    Serial.println("Display connection started");
    display.fillScreen(BACKGROUND_COLOR);
    display.setTextWrap(false);
    display.setRotation(ROTATION);
    Serial.println("Display initialized");
    Serial.println("Touch calibration started");
    dPrint("Calibration", round(DISPLAY_LENGTH/2), round(DISPLAY_HEIGHT/2), TOUCH_CALIBRATION_HEADER_SIZE, TEXT_COLOR, 7);
    dPrint("press blue box", round(DISPLAY_LENGTH/2), round(DISPLAY_HEIGHT/2), TOUCH_CALIBRATION_TEXT_SIZE, TEXT_COLOR, 1);
    ts = TouchScreen(TFT_D1, TFT_DC, TFT_CS, TFT_D0, display, TOUCH_RESISTANCE);
    Serial.print(ts.xmin); Serial.print(", "); Serial.print(ts.xmax); Serial.print(", "); Serial.print(ts.ymin); Serial.print(", "); Serial.println(ts.ymax);
    Serial.println("Touch calibrated");
    loadingScreen();
    start = true;
    Serial.println("DISPLAY SETUP complete");
    Serial.println();
  }

  TFT_eSPI DisplayVX::getDisplay() {
    return(display);
  }

  //Loading Screen
  void DisplayVX::loadingScreen() {
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

  extern void DisplayVX::writeLoadingScreenTitle() {
    short distanceToFirstLetter = (DISPLAY_LENGTH - (18*LOADING_SCREEN_TITLE_SIZE)) / 2;
    short distanceToFirstLetterSub = (DISPLAY_LENGTH - (30*LOADING_SCREEN_SUB_SIZE)) / 2;
    dPrint("A", distanceToFirstLetter + LOADING_SCREEN_TITLE_SIZE, 35, LOADING_SCREEN_TITLE_SIZE, LIGHT_BLUE);
    dPrint("I", distanceToFirstLetter + 6*LOADING_SCREEN_TITLE_SIZE, 35, LOADING_SCREEN_TITLE_SIZE, TURKISE);
    dPrint("R", distanceToFirstLetter + 12*LOADING_SCREEN_TITLE_SIZE, 35, LOADING_SCREEN_TITLE_SIZE, LIME);
    dPrint("duino", distanceToFirstLetterSub, 120, LOADING_SCREEN_SUB_SIZE, GREY);
  }

  extern void DisplayVX::handleTouch() {
    // a point object holds x y and z coordinates
    TSPoint p = ts.getPoint();
    // we have some minimum pressure we consider 'valid'
    // pressure of 0 means no pressing!
    if (p.isTouching()) {
      p.calibrate();
      p.print();
      if(p.isTouching(MENU_START_X, MENU_END_X, MENU_START_Y, MENU_END_Y)) {
        display.fillRect(MENU_START_X, MENU_START_Y, MENU_LENGTH, MENU_HEIGHT, RED);
        Serial.println("open Menu!!!");
      } else {
          display.fillRect(MENU_START_X, MENU_START_Y, MENU_LENGTH, MENU_HEIGHT, BACKGROUND_COLOR);
          display.drawRect(MENU_START_X, MENU_START_Y, MENU_LENGTH, MENU_HEIGHT, RED);
      }
    }
    display.pushImage(MENU_START_X, MENU_START_Y, MENU_LENGTH, MENU_HEIGHT, menuGear, WHITE);
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

  extern void DisplayVX::drawDisplay() {
    display.fillScreen(BACKGROUND_COLOR);
    display.fillRect(0, DATABOX_Y, DISPLAY_LENGTH, DATABOX_HEIGHT, DATABOX_BACKGROUND_COLOR);
    Serial.println("Display drawn");
  }

  extern void DisplayVX::getData() {
    state = Meassure::getState();
    airCondition = Meassure::getAirCondition();
  }

  extern void DisplayVX::generateData(int startPPM, int endPPM, int changePPM) {
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

  extern void DisplayVX::checkState() {
    if (blinkSwitch) {
      drawBorder(0, 0, DISPLAY_LENGTH, DISPLAY_HEIGHT, 1, BACKGROUND_COLOR);
      display.drawLine(0, DISPLAY_HEIGHT-1, DISPLAY_LENGTH-1, DISPLAY_HEIGHT-1, DATABOX_BACKGROUND_COLOR);
      display.drawLine(0, DATABOX_Y, 0, DISPLAY_HEIGHT-1, DATABOX_BACKGROUND_COLOR);
      display.drawLine(DISPLAY_LENGTH-1, DATABOX_Y, DISPLAY_LENGTH-1, DISPLAY_HEIGHT-1, DATABOX_BACKGROUND_COLOR);
      display.drawLine(0, DATABOX_BAR_Y, 0, DATABOX_Y-1, state.getColor(COLORED_BAR));
      display.drawLine(DISPLAY_LENGTH-1, DATABOX_BAR_Y, DISPLAY_LENGTH-1, DATABOX_Y-1, state.getColor(COLORED_BAR));
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
  extern void DisplayVX::writeInfo() {
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

    //Verhindert überschreiben von "ppm"
    if (airCondition < 1000 && lastAirCondition >= 1000 || airCondition < 1000 && start) {
      dPrint(String(airCondition) + " ", PPM_MARGIN_LEFT, PPM_Y, PPM_SIZE, state.getColor(COLORED_PPM), 6, DATABOX_BACKGROUND_COLOR, String(lastAirCondition));
      dPrint("ppm", PPM_STRING_X, PPM_STRING_Y, PPM_STRING_SIZE, state.getColor(COLORED_PPM), 6);
    } else if(airCondition >= 1000 && lastAirCondition < 1000)
      dPrint("ppm", PPM_STRING_X, PPM_STRING_Y, PPM_STRING_SIZE, DATABOX_BACKGROUND_COLOR, 6);

    //write new Pixels
    dPrint(airCondition, PPM_MARGIN_LEFT, PPM_Y, PPM_SIZE, state.getColor(COLORED_PPM), 6, DATABOX_BACKGROUND_COLOR, lastAirCondition);

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

  extern void DisplayVX::drawBorder(int x, int y, int length, int height, int thickness, int color) {
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


  extern void DisplayVX::drawLine(int x, int y, int length, int height, int color, int a, int b, boolean filter1, boolean filter2) {
    length += x;
    height += y;
    if(a > 1)
      x += floor((length-1-x)%a/2);
    if(b > 1)
      y += floor((height-1-y)%b/2);
    for (int i = x; i < length; i+=a) {
      for (int j = y; j < height; j+=b) {
        if(filter1) {
          if(display.readPixel(i, j) == (0x0FFFU * BACKGROUND_COLOR)/100) {
            if(filter2 && airCondition >= DISPLAYED_PPM_HIGHEST)
              display.drawPixel(i, j, state.getColor(COLORED_BAR));
            else
              display.drawPixel(i, j, color);
          }
        } else
          display.drawPixel(i, j, color);
      }
    }
  }

  extern void DisplayVX::showBoxes() {
    display.fillRect(PPM_MARGIN_LEFT, DISPLAY_HEIGHT - PPM_HEIGHT, PPM_LENGTH, PPM_HEIGHT, GREEN);
    display.fillRect(DISPLAY_LENGTH - TIMER_LENGTH - TIMER_MARGIN_RIGHT, DISPLAY_HEIGHT - TIMER_MARGIN_BOTTOM - TIMER_HEIGHT, TIMER_LENGTH, TIMER_HEIGHT, WHITE);
    display.fillRect(DISPLAY_LENGTH - TIMER_LENGTH - TIMER_MARGIN_RIGHT, DISPLAY_HEIGHT - TIMER_MARGIN_BOTTOM - TIMER_SIZE*LETTER_HEIGHT, TIMER_LENGTH, TIMER_SIZE*LETTER_HEIGHT, BLUE);
    display.fillRect((DISPLAY_LENGTH - STATUS_LENGTH)/2, STATUS_MARGIN_TOP, STATUS_LENGTH, STATUS_HEIGHT, RED);
  }

  extern void DisplayVX::dPrint(String text, int x, int y, int scale, int color, int datum, int backgroundColor, String oldText, int padding) {
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
  extern void DisplayVX::dPrint(int text, int x, int y, int scale, int color, int datum, int backgroundColor, int oldText, int padding) {
    dPrint(String(text), x, y, scale, color, datum, backgroundColor, (oldText == -1) ? "" : String(oldText), padding);
  }

  extern void DisplayVX::drawLoadingBar() {

  }
