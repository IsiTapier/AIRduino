/*
  DisplayV1.h - DisplayV1-Library for Airduino.
*/

#ifndef DisplayV1_h
#define DisplayV1_h

#include "../DisplayVX/Display.h"

//Design
#define MIN_HIGHT DISPLAY_HEIGHT - map(Util::calibration[EEPROM.read(0)].getLowestPPM(), 0, MAX_DISPLAYED_PPM, 0, DISPLAY_HEIGHT)
#define CRITICAL_HIGHT DISPLAY_HEIGHT - map(MAX_LIGHT, 0, MAX_DISPLAYED_PPM, 0, DISPLAY_HEIGHT)

#define AVERAGING_GRAPH 10
#define ALPHA_GRAPH 0.7

#define MAX_LIGHT 1000
#define OSV_PPM 400
#define MAX_DISPLAYED_PPM 1280

class DisplayV1 : public Display {

  public:
    static void setup();
    static void loop();

  private:
    static boolean averageData();
    static void fillData();
    static void createLines();
    static void drawGraph();
    static void drawLoadingBar();
    static void drawConnections(int x, int startY, int endY);

    static int valuesGraph[AVERAGING_GRAPH];
    static int graphData[DISPLAY_LENGTH];
    static int counter;
    static int pixel;
    static int lastPixel;

};

#endif
