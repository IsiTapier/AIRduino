 /*
  DisplayV1.cpp - DisplayV1-Library for Librarys.
*/
#include "Arduino.h"
#include "DisplayV1.h"

int DisplayV1::valuesGraph[100];
int DisplayV1::graphData[DISPLAY_LENGTH+1];
int DisplayV1::counter = -1;
int DisplayV1::currentPosition = 0;
int DisplayV1::pixel = 0;
int DisplayV1::lastPixel = 0;

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

  void DisplayV1::setup() {
    DisplayVX::setup();
    debug(DEBUG, SETUP, "DisplayV1 SETUP started");
    graphData[0] = GRAPH_START_Y;
    debug(DEBUG, SETUP, "DisplayV1 SETUP completed");
    debug(DEBUG, SETUP, "");
    if(general::mode.equaled(LOADINGSCREEN))
      loop(false);
  }

  boolean DisplayV1::getGraphData() {
    //general::mode.equals(CHART) && general::mode.equaled(CHART)
    DisplayVX::handleData();
    if(averageData()) {
      fillData();
      return true;
    }
    if(state.getColor(COLORED_CHART) != lastState.getColor(COLORED_CHART) && !COLOR_MODE)
      return true;
    return false;
  }

  void DisplayV1::loop(boolean changed) {
    if(start) {
      drawDisplay();
      createLines();
      DisplayVX::handleData();
      if(general::mode.equals(CHART)) {
        DisplayVX::writeInfo();
        DisplayVX::checkState();
      }
      drawGraph();
    } else {
      if(general::mode.equals(CHART) && general::mode.equaled(CHART)) {
        DisplayVX::writeInfo();
        DisplayVX::checkState();
      }
    }
    //graph
    if(changed)
      drawGraph();
    
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

  boolean DisplayV1::averageData() {
    valuesGraph[counter] = airCondition;
    counter ++;
    if (!(counter < AVERAGING_GRAPH)) {
      lastPixel = pixel;
      pixel = average(valuesGraph, 0, AVERAGING_GRAPH);
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

  void DisplayV1::fillData() {
    if(currentPosition < DISPLAY_LENGTH) {
      currentPosition++;
      graphData[currentPosition] = pixel;
    } else {
      if(graphData[0] != graphData[1])
        drawConnection(1, state >= BLINK && blinkSwitch ? TEXT_COLOR : BACKGROUND_COLOR);
      for (short x = 0; x < DISPLAY_LENGTH; x++) {
        graphData[x] = graphData[x + 1];
      }
      graphData[DISPLAY_LENGTH] = pixel;
    }
  }

  void DisplayV1::createLines() {
    drawLine(0, FIRST_SECTION_Y, DISPLAY_LENGTH, 1, GRAPH_COLOR, GRAPH_SECTIONS_STRIPE_DISTANCE, 1, true, airCondition);
    drawLine(0, SECOND_SECTION_Y, DISPLAY_LENGTH, 1, GRAPH_COLOR, GRAPH_SECTIONS_STRIPE_DISTANCE, 1, true, airCondition);
  //  drawLine(0, TOP_BAR_HEIGHT, DISPLAY_LENGTH, TOP_BAR_THICKNESS, GRAPH_COLOR, 1, 1, true, airCondition, true, state);
  }

  void DisplayV1::drawGraph() {
    /*//draw Graph Top
    if(COLORED_BAR && airCondition >= DISPLAYED_PPM_HIGHEST && lastAirCondition < DISPLAYED_PPM_HIGHEST)
      display.drawLine(0, GRAPH_END_Y, DISPLAY_LENGTH, GRAPH_END_Y, state.getColor(true));
    if(COLORED_BAR && airCondition < DISPLAYED_PPM_HIGHEST && lastAirCondition >= DISPLAYED_PPM_HIGHEST)
      display.drawLine(0, GRAPH_END_Y, DISPLAY_LENGTH, GRAPH_END_Y, state.getColor(false));*/
    //draw Graph
    if(currentPosition >= DISPLAY_LENGTH-1 || (state.getColor(COLORED_CHART) != lastState.getColor(COLORED_CHART) && !COLOR_MODE) /*|| COLORED_BAR && airCondition < DISPLAYED_PPM_HIGHEST && lastAirCondition >= DISPLAYED_PPM_HIGHEST || general::mode.hasChanged() && general::mode.equals(CHART)*/ || start) {
      for (short x = 1; x <= currentPosition; x++) {
        if(state < BLINK || !blinkSwitch || x != 1)
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

  void DisplayV1::drawConnection(int x, int color, int shift) {
    //return if wrong arguments
    if(x < 1)
      return;
    if(x+shift > DISPLAY_LENGTH)
      return;
    //get data
    int i;
    int j;
    if(graphData[x] < graphData[x-1]) {
      i = graphData[x]+1;
      j = graphData[x-1]+1;
    } else {
      i = graphData[x-1];
      j = graphData[x];
    }
    //value correction
    /*if(i < GRAPH_END_Y)
      i = GRAPH_END_Y;*/
    if(i < TOP_BAR_HEIGHT+DATABOX_BAR_THICKNESS)
      i = TOP_BAR_HEIGHT+DATABOX_BAR_THICKNESS;
    if(j > GRAPH_START_Y)
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
        color_ = getStateOf(map(y-2, GRAPH_START_Y, GRAPH_END_Y, DISPLAYED_PPM_LOWEST, DISPLAYED_PPM_HIGHEST)).getColor(COLORED_CHART);
      //draw connection
      display.drawPixel(x-1, y, color_);
    }
  }

  void DisplayV1::resetGraph() {
    currentPosition = 0;
    counter = -1;
  }
