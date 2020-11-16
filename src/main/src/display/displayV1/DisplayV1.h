/*
  DisplayV1.h - DisplayV1-Library for Airduino.
*/

#ifndef DisplayV1_h
#define DisplayV1_h

#include "../DisplayVX/Display.h"

//Design
#define MIN_HIGHT DISPLAY_WIDTH - map(OSV_PPM, 0, MAX_DISPLAYED_PPM, 0, DISPLAY_WIDTH)
#define CRITICAL_HIGHT DISPLAY_WIDTH - map(MAX_LIGHT, 0, MAX_DISPLAYED_PPM, 0, DISPLAY_WIDTH)

class DisplayV1 : public Display {

  public:
    static void setup();
    static void loop();

  private:
    static boolean getData();
    static void fillData();
    static void createLines();
    static void drawGraph();
    static void drawConnections(int x, int startY, int endY);

    static int valuesGraph[AVERAGING_GRAPH];
    static int graphData[DISPLAY_LENGTH];
    static int counter;
    static int pixel;
    static int lastPixel;

};

#endif
