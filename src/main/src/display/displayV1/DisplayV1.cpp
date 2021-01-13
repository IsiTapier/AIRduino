 /*
  DisplayV1.cpp - DisplayV1-Library for Librarys.
*/
#include "Arduino.h"
#include "DisplayV1.h"

extern int DisplayV1::valuesGraph[AVERAGING_GRAPH];
extern int DisplayV1::graphData[DISPLAY_LENGTH+1];
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
    DisplayVX::setup();

    Serial.println("DisplayV1-Setup started");
    graphData[0] = GRAPH_START_Y;
    Serial.println("DisplayV1-Setup complete");
    Serial.println();
  }

  extern void DisplayV1::loop() {
    getData();
    //generateData(400, 1100, 5);
    //setup
    if(start) {
      drawDisplay();
      createLines();
    }
    //graph
    if (averageData()) {
      fillData();
      drawGraph();
    }
    //info
    writeInfo();
    checkState();
    handleTouch();
    //end setup
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
      pixel = map(pixel, DISPLAYED_PPM_LOWEST, DISPLAYED_PPM_HIGHEST, GRAPH_START_Y, GRAPH_END_Y);
      if(lastPixel != 0)
        pixel = ALPHA_GRAPH * pixel + (1 - ALPHA_GRAPH) * lastPixel;
      if(pixel > GRAPH_START_Y)
        pixel = GRAPH_START_Y;
      if(pixel < GRAPH_END_Y)
        pixel = GRAPH_END_Y;
      counter = 0;
      return (true);
    } else
      return (false);
  }

  extern void DisplayV1::fillData() {
    if(currentPosition < DISPLAY_LENGTH) {
      currentPosition++;
      graphData[currentPosition] = pixel;
    } else {
      if(graphData[0] != graphData[1])
        drawConnection(1, BACKGROUND_COLOR);
      for (short x = 0; x < DISPLAY_LENGTH; x++) {
        graphData[x] = graphData[x + 1];
      }
      graphData[DISPLAY_LENGTH] = pixel;
    }
  }

  extern void DisplayV1::createLines() {
    drawLine(0, FIRST_SECTION_Y, DISPLAY_LENGTH, 1, GRAPH_COLOR, GRAPH_SECTIONS_STRIPE_DISTANCE, 1, true);
    drawLine(0, SECOND_SECTION_Y, DISPLAY_LENGTH, 1, GRAPH_COLOR, GRAPH_SECTIONS_STRIPE_DISTANCE, 1, true);
    drawLine(0, GRAPH_END_Y, DISPLAY_LENGTH, 1, GRAPH_COLOR, 1, 1, true, true);
  }

  extern void DisplayV1::drawGraph() {
    //draw Graph Top
    if(COLORED_BAR && airCondition >= DISPLAYED_PPM_HIGHEST && lastAirCondition < DISPLAYED_PPM_HIGHEST)
      display.drawLine(0, GRAPH_END_Y, DISPLAY_LENGTH, GRAPH_END_Y, state.getColor(true));
    if(COLORED_BAR && airCondition < DISPLAYED_PPM_HIGHEST && lastAirCondition >= DISPLAYED_PPM_HIGHEST)
      display.drawLine(0, GRAPH_END_Y, DISPLAY_LENGTH, GRAPH_END_Y, state.getColor(false));
    //draw Graph
    if(currentPosition >= DISPLAY_LENGTH-1 || state.getColor(COLORED_CHART) != lastState.getColor(COLORED_CHART) && !COLOR_MODE || COLORED_BAR && airCondition < DISPLAYED_PPM_HIGHEST && lastAirCondition >= DISPLAYED_PPM_HIGHEST) {
      for (short x = 1; x <= currentPosition; x++) {
        drawConnection(x);
        if(currentPosition >= DISPLAY_LENGTH)
          drawConnection(x, BACKGROUND_COLOR, 1);
      }
      if(currentPosition >= DISPLAY_LENGTH)
        createLines();
    } else {
      drawConnection(currentPosition);
    }
  }

  extern void DisplayV1::drawConnection(int x, int color, int shift) {
    //return if wrong arguments
    if(x < 1)
      return;
    if(x+shift > DISPLAY_LENGTH)
      return;
    //get data
    int i;
    int j;
    if(graphData[x] < graphData[x-1]) {
      i = graphData[x];
      j = graphData[x-1];
    } else {
      i = graphData[x-1];
      j = graphData[x];
    }
    //value correction
    if(i < GRAPH_END_Y)
      i = GRAPH_END_Y;
    if(j >= GRAPH_START_Y)
      j = GRAPH_START_Y;
    //get color
    int color_;
    if(color < 0 && !COLOR_MODE)
      color_ = state.getColor(COLORED_CHART);
    else
      color_ = color;
    //shift position
    x += shift;
    //draw connections
    for(int y = i; y <= j; y++) {
      //get color
      if(COLOR_MODE && color < 0)
        color_ = Util::getStateOf(map(y-2, GRAPH_START_Y, GRAPH_END_Y, DISPLAYED_PPM_LOWEST, DISPLAYED_PPM_HIGHEST)).getColor(COLORED_CHART);
      //draw connection
      display.drawPixel(x-1, y, color_);
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
