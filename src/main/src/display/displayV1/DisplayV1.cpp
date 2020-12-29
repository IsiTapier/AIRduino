 /*
  DisplayV1.cpp - DisplayV1-Library for Librarys.
*/
#include "Arduino.h"
#include "DisplayV1.h"

extern int DisplayV1::valuesGraph[AVERAGING_GRAPH];
extern int DisplayV1::graphData[DISPLAY_LENGTH];
extern int DisplayV1::counter = 0;
extern int DisplayV1::currentPosition = 0;
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

    Serial.println("DisplayV1-Setup complete");
    Serial.println();
  }

  extern void DisplayV1::loop() {
    getData();
    //generateData(400, 1000, 30);

    if(start) {
      drawDisplay();
    }

    if (averageData()) {
      fillData();
      createLines();
      drawGraph();
    }
    writeInfo();
    checkState();

    if(start)
      start = false;
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

  extern boolean DisplayV1::averageData() {
    valuesGraph[counter] = airCondition;
    counter ++;
    if (!(counter < AVERAGING_GRAPH)) {
      lastPixel = pixel;
      pixel = Util::average(valuesGraph, 0, AVERAGING_GRAPH);
      pixel = ALPHA_GRAPH * pixel + (1 - ALPHA_GRAPH) * lastPixel;
      pixel = map(pixel, DISPLAYED_PPM_LOWEST, DISPLAYED_PPM_HIGHEST, GRAPH_START_Y, GRAPH_END_Y);
      if(pixel <= DATABOX_BAR_Y)
        pixel = DATABOX_BAR_Y-1;
      counter = 0;
      return (true);
    } else
      return (false);
  }

  // Künstliches Auffüllen der Werte, wird später vom Modul übernommen
  extern void DisplayV1::fillData() {
    if(currentPosition == DISPLAY_LENGTH-1) {
      for (short x = 0; x < DISPLAY_LENGTH; x++) {
        graphData[x] = graphData[x + 1];
      }
      graphData[DISPLAY_LENGTH] = pixel;
    } else {
      graphData[currentPosition] = pixel;
    }
  }

  extern void DisplayV1::createLines() {
    drawLine(DISPLAY_LENGTH + 1, FIRST_SECTION_Y, 5, true);
    drawLine(DISPLAY_LENGTH + 1, SECOND_SECTION_Y, 5, true);
  }

  extern void DisplayV1::drawGraph() {
    display.drawLine(DISPLAY_LENGTH, 0, DISPLAY_LENGTH, DATABOX_Y - 1, BACKGROUND_COLOR);
    for (short x = 0; x <= DISPLAY_LENGTH; x++) {
      byte arrayDigit = x;
      if (graphData[arrayDigit] < DATABOX_Y)
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
      if (y < DATABOX_Y) {
        display.drawPixel(x - 1, y, BACKGROUND_COLOR);
        display.drawPixel(x, y, GRAPH_COLOR);
      }
    }
  }

  extern void DisplayV1::drawLoadingBar() {
    display.fillRect(0, DATABOX_Y, DISPLAY_LENGTH + 1, DATABOX_BAR_THICKNESS, state.getColor(true));
    //Draw Loading Bar
    if (state == VENTILATING) {
      short bar_ventilating_length = map(airCondition, Meassure::getLowest(), Meassure::getPpmSinceVentilation(), 0, DISPLAY_LENGTH);
      display.fillRect(DISPLAY_LENGTH - bar_ventilating_length, DATABOX_Y, bar_ventilating_length, DATABOX_BAR_THICKNESS, GREY);
    }
  }
