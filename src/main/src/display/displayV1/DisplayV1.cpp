 /*
  DisplayV1.cpp - DisplayV1-Library for Librarys.
*/
#include "Arduino.h"
#include "DisplayV1.h"

extern int DisplayV1::valuesGraph[AVERAGING_GRAPH];
extern int DisplayV1::graphData[DISPLAY_LENGTH];
extern int DisplayV1::counter = 0;
extern int DisplayV1::pixel = 0;
extern int DisplayV1::lastPixel = 0;

  //   _____      _
  //  / ____|    | |
  // | (___   ___| |_ _   _ _ __
  //  \___ \ / _ \ __| | | | '_ \
  //  ____) |  __/ |_| |_| | |_) |
  // |_____/ \___|\__|\__,_| .__/
  //                       | |
  //                       |_|

  extern void DisplayV1::setup() {
    Display::setup();

    Serial.println("DisplayV1-Setup started");
    //Auffüllen des Arrays
    for (short x = 0; x < DISPLAY_LENGTH; x++) {
      graphData[x] = DISPLAY_LENGTH;
    }
    graphData[0] = DATABOX_TOP_HIGHT - 1;
    Serial.println("DisplayV1-Setup complete");
    Serial.println();
  }

  extern void DisplayV1::loop() {
    state = Meassure::getState();
    airCondition = Meassure::getAirCondition();

    checkState();
    if (getData()) {
      createLines();
      fillData();
      drawGraph();
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

  extern boolean DisplayV1::getData() {
    valuesGraph[counter] = airCondition;
    counter ++;
    if (!(counter < AVERAGING_GRAPH)) {
      pixel = Util::average(valuesGraph, 0, AVERAGING_GRAPH);
      pixel = ALPHA_GRAPH * pixel + (1 - ALPHA_GRAPH) * lastPixel;
      lastPixel = pixel;
      counter = 0;
      return (true);
    } else
      return (false);
  }

  // Künstliches Auffüllen der Werte, wird später vom Modul übernommen
  extern void DisplayV1::fillData() {
    for (short x = DISPLAY_LENGTH; x > 0; x--) {
      graphData[x] = graphData[x - 1];
    }
    graphData[0] = DISPLAY_WIDTH - pixel;
  }

  extern void DisplayV1::createLines() {
    display.fillRect(0, DATABOX_TOP_HIGHT, DISPLAY_LENGTH + 1, DISPLAY_WIDTH, BAR_BACKGROUND_COLOR);
    drawLine(DISPLAY_LENGTH + 1, CRITICAL_HIGHT, 5);
    drawLine(DISPLAY_LENGTH + 1, MIN_HIGHT, 5);
  }

  extern void DisplayV1::drawGraph() {
    display.drawLine(DISPLAY_LENGTH, 0, DISPLAY_LENGTH, DATABOX_TOP_HIGHT - 1, GRAPH_BACKGROUND_COLOR);
    for (short x = 0; x <= DISPLAY_LENGTH; x++) {
      byte arrayDigit = x;
      if (graphData[arrayDigit] < DATABOX_TOP_HIGHT)
        display.drawPixel(x, graphData[arrayDigit], GRAPH_COLOR);
      if (x != 0) {
        if (graphData[arrayDigit - 1] < graphData[arrayDigit])
          drawConnections(x, graphData[arrayDigit - 1] + 1, graphData[arrayDigit] + 1);
        else
          drawConnections(x, graphData[arrayDigit], graphData[arrayDigit - 1]);
      }
    }
  }

  extern void DisplayV1::drawConnections(int x, int startY, int endY) {
    for (byte y = startY; y < endY; y++) {
      if (y < DATABOX_TOP_HIGHT) {
        display.drawPixel(x - 1, y, GRAPH_BACKGROUND_COLOR);
        display.drawPixel(x, y, GRAPH_COLOR);
      }
    }
  }

  extern void DisplayV1::drawLoadingBar() {
    display.fillRect(0, DATABOX_TOP_HIGHT, DISPLAY_LENGTH + 1, BAR_STRIPE_THICKNESS, Util::getColor(state));
    //Draw Loading Bar
    if (state == VENTILATING) {
      short bar_ventilating_length = map(airCondition, Meassure::getLowest(), Meassure::getPpmSinceVentilation(), 0, DISPLAY_LENGTH);
      display.fillRect(DISPLAY_LENGTH - bar_ventilating_length, DATABOX_TOP_HIGHT, bar_ventilating_length, BAR_STRIPE_THICKNESS, GREY);
    }
  }
