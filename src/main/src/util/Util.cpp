/*
  Util.cpp - Util-Library for Librarys.
*/

#include "Arduino.h"
#include "Util.h"

  TFT_eSPI display(DISPLAY_HEIGHT, DISPLAY_LENGTH);
  TouchScreen ts(TFT_D1, TFT_DC, TFT_CS, TFT_D0, TOUCH_RESISTANCE);

  Calibration calibration[] = {
      Calibration(189, 216, 400, 975),//(449, 714, 576, 1184), //576 1424 496 478 //216 875 //189 400
      Calibration(710, 784, 550, 900),
      Calibration(40, 60, 400, 1000)
  };

  //State
  State getStateOf(int value) {
         if (value <= LIMIT_GOOD)
      return((State) GOOD);
    else if (value <= LIMIT_MEDIUM)
      return((State) MEDIUM);
    else if (value < LIMIT_BLINK)
      return((State) BAD);
    else if (value >= LIMIT_BLINK)
      return((State) BLINK);
    else if (value >= LIMIT_PIEP)
      return((State) PIEP);
    else
      return((State) BAD);
  }

  /*long map(double x, long in_min, long in_max, long out_min, long out_max) {
    return round((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
  }*/

  int map(int value, int startA, int startB, float factor) {
    value = value - startA;
    value = value * factor;
    value = value + startB;
    return(value);
  }

  int average(int averageArray[], int averageStart, int averageEnd) {
    int sum = 0;
    for (int i = averageStart; i < averageEnd; i++) {
      sum = sum + averageArray[i];
    }
    return (sum / (averageEnd-1 - averageStart));
  }

  void drawBorder(int x, int y, int length, int height, int thickness, int color) {
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

  void drawLine(int x, int y, int length, int height, int color, int a, int b, boolean filter1, int filtervalue1, boolean filter2, State filtervalue2) {
    length += x;
    height += y;
    if(a > 1)
      x += floor((length-1-x)%a/2);
    if(b > 1)
      y += floor((height-1-y)%b/2);
    for (int i = x; i < length; i+=a) {
      for (int j = y; j < height; j+=b) {
        if(filter1) {
          if(display.readPixel(i, j) == (BACKGROUND_COLOR/* 0xFFFFU * BACKGROUND_COLOR */)/* /100 */) {
            if(filter2 && filtervalue1 >= DISPLAYED_PPM_HIGHEST)
              display.drawPixel(i, j, filtervalue2.getColor(COLORED_BAR));
            else
              display.drawPixel(i, j, color);
          }
        } else
          display.drawPixel(i, j, color);
      }
    }
  }

  void showBoxes() {
    display.fillRect(PPM_MARGIN_LEFT, DISPLAY_HEIGHT - PPM_HEIGHT, PPM_LENGTH, PPM_HEIGHT, GREEN);
    display.fillRect(DISPLAY_LENGTH - TIMER_LENGTH - TIMER_MARGIN_RIGHT, DISPLAY_HEIGHT - TIMER_MARGIN_BOTTOM - TIMER_HEIGHT, TIMER_LENGTH, TIMER_HEIGHT, WHITE);
    display.fillRect(DISPLAY_LENGTH - TIMER_LENGTH - TIMER_MARGIN_RIGHT, DISPLAY_HEIGHT - TIMER_MARGIN_BOTTOM - TIMER_SIZE*LETTER_HEIGHT, TIMER_LENGTH, TIMER_SIZE*LETTER_HEIGHT, BLUE);
    display.fillRect((DISPLAY_LENGTH - STATUS_LENGTH)/2, STATUS_MARGIN_TOP, STATUS_LENGTH, STATUS_HEIGHT, RED);
  }

  void dPrint(String text, int x, int y, int size, int color, int datum, int backgroundColor, String oldText, int oldTextSize, int padding) {
    display.setTextPadding(padding);
    display.setTextDatum(datum);
    if(backgroundColor == 0)
      backgroundColor = BACKGROUND_COLOR;
    if(oldText.length() != 0) {
      if(oldTextSize < 0)
        oldTextSize = size;
      int xold = x;
      if(datum % 3 == 1) {
        x -= size*LETTER_LENGTH*text.length()/2;
        xold -= oldTextSize*LETTER_LENGTH*oldText.length()/2;
        display.setTextDatum(datum-1);
      } else if(datum % 3 == 2) {
        x -= LETTER_LENGTH*size*text.length();
        xold -= LETTER_LENGTH*oldTextSize*oldText.length();
        display.setTextDatum(datum-2);
      }
      display.setTextColor(backgroundColor);
      display.setTextSize(oldTextSize);
      for(int i = 0; i < oldText.length(); i++) {
        if(text.charAt(i) != oldText.charAt(i) || text.length() != oldText.length()) {
          display.drawString((String) oldText.charAt(i), (xold+LETTER_LENGTH*oldTextSize*i), y);
        }
      }
      display.setTextColor(color);
      display.setTextSize(size);
      for(int i = 0; i < text.length(); i++) {
        if(text.charAt(i) != oldText.charAt(i) || text.length() != oldText.length()) {
          display.drawString((String) text.charAt(i), x+LETTER_LENGTH*size*i, y);
        }
      }
    } else {
      display.setTextSize(size);
      display.setCursor(x, y);
      if(backgroundColor > 0)
        display.setTextColor(color, backgroundColor);
      else
        display.setTextColor(color);
      display.drawString(text);
    }
  }

  //Verkürzung: Writing mit Integern
  void dPrint(int text, int x, int y, int size, int color, int datum, int backgroundColor, int oldText, int oldTextSize, int padding) {
    dPrint(String(text), x, y, size, color, datum, backgroundColor, (oldText == -1) ? "" : String(oldText), oldTextSize, padding);
  }
  //Loading Screen
  void loadingScreen() {
    debug(DEBUG, SETUP, "loadingscreen started");
    display.fillScreen(BACKGROUND_COLOR);
    writeLoadingScreenTitle();
    debug(DEBUG, SETUP, "loadingscreen ended");
  }

  void writeLoadingScreenTitle() {
    short distanceToFirstLetterSub = 52;
    short distanceToFirstDot = 90;
    short DotVerticalPosition = 155;
    byte DD = DD_MARGIN; //3d Versatz


    dPrint("duino", distanceToFirstLetterSub + 2, 120 + 2, LOADING_SCREEN_SUB_SIZE, DARKGREY);
    dPrint("duino", distanceToFirstLetterSub, 120, LOADING_SCREEN_SUB_SIZE, GREY);

    //Draw: Loading Dots
    byte c = 0;
    for (int x = LOADING_SCREEN_TIME * 2; x >= 0; x--) {
      c++;
      dPrint("...", distanceToFirstDot, DotVerticalPosition, LOADING_SCREEN_DOT_SIZE, BACKGROUND_COLOR); // Altes Clearen
      dPrint("...", distanceToFirstDot + DD, DotVerticalPosition + DD, LOADING_SCREEN_DOT_SIZE, BACKGROUND_COLOR);
      switch (c) {
        case 1:
            dPrint(".", distanceToFirstDot + DD, DotVerticalPosition + DD, LOADING_SCREEN_DOT_SIZE, GREY);
            dPrint(".",distanceToFirstDot, DotVerticalPosition, LOADING_SCREEN_DOT_SIZE, LOADING_SCREEN_DOTS_COLOR);

            writeLoadingAnimation(LIGHT_BLUE, TURKISE, LIME);
          break;
        case 2:
            dPrint("..", distanceToFirstDot + DD, DotVerticalPosition + DD, LOADING_SCREEN_DOT_SIZE, GREY);
            dPrint("..", distanceToFirstDot, DotVerticalPosition, LOADING_SCREEN_DOT_SIZE, LOADING_SCREEN_DOTS_COLOR);

            writeLoadingAnimation(TURKISE, LIME, LIGHT_BLUE);
          break;
        case 3:
          dPrint("...", distanceToFirstDot + DD, DotVerticalPosition + DD, LOADING_SCREEN_DOT_SIZE, GREY);
          dPrint("...", distanceToFirstDot, DotVerticalPosition, LOADING_SCREEN_DOT_SIZE, LOADING_SCREEN_DOTS_COLOR);

          writeLoadingAnimation(LIME, LIGHT_BLUE, TURKISE);
          c = 0;
          break;
      }
      delay(500);
    }

  }

  void writeLoadingAnimation(int c1, int c2, int c3) {
    long endTime = millis() + LOADING_SCREEN_TIME * 1000;

    short distanceToFirstLetter = 70 + DD_MARGIN;
    short VerticalDistanceToFirstLetter = 55 + DD_MARGIN;
    dPrint("A", distanceToFirstLetter, VerticalDistanceToFirstLetter, LOADING_SCREEN_TITLE_SIZE, GREY);
    dPrint("I", distanceToFirstLetter + 60, VerticalDistanceToFirstLetter, LOADING_SCREEN_TITLE_SIZE, GREY);
    dPrint("R", distanceToFirstLetter + 120, VerticalDistanceToFirstLetter, LOADING_SCREEN_TITLE_SIZE, GREY);

    distanceToFirstLetter = distanceToFirstLetter - DD_MARGIN;
    VerticalDistanceToFirstLetter = VerticalDistanceToFirstLetter - DD_MARGIN;
    dPrint("A", distanceToFirstLetter, 55, LOADING_SCREEN_TITLE_SIZE, c1);
    dPrint("I", distanceToFirstLetter + 60, 55, LOADING_SCREEN_TITLE_SIZE, c2);
    dPrint("R", distanceToFirstLetter + 120, 55, LOADING_SCREEN_TITLE_SIZE, c3);
  }

  boolean requestDecision(String topic, String question, String op1, String op2) {
    display.fillScreen(BACKGROUND_COLOR);
    dPrint(topic, 160, 40, 2, TEXT_COLOR, 4);
    dPrint(question, 160, 60, 2, TEXT_COLOR, 4);
    dPrint(op1, 0, 240, 4, GREEN, 6);
    dPrint(op2, 320, 240, 4, RED, 8);
    TSPoint p = ts.getPoint();
    while(true) {
      while(p.isTouching()) {
        p = ts.getPoint();
        delay(100);
      }
      delay(400);
      p = ts.getPoint();
      if(p.isTouching()) {
        p.calibrate();
        if(p.isTouching(0, 160, 120, 240)) {
          // Serial.println("true");
          return true;
        }
        if(p.isTouching(161, 320, 120, 240)) {
          // Serial.println("false");
          return false;
        }
        delay(10);
      }
    }
  }