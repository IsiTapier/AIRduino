/*
  DisplayV1.h - DisplayV1-Library for Airduino.
*/

#ifndef DisplayV1_h
#define DisplayV1_h

#include "../DisplayVX/DisplayVX.h"

class DisplayV1 : public DisplayVX {

  public:
    static void setup();
    static void loop(boolean changed);
    static boolean getGraphData();

  private:
    static boolean averageData();
    static void createLines();
    static void fillData();
    static void drawGraph();
    static void drawConnection(int x, int color=-1, int shift = 0);

    static int valuesGraph[AVERAGING_GRAPH];
    static int graphData[DISPLAY_LENGTH+1];
    static int counter;
    static int currentPosition;
    static int pixel;
    static int lastPixel;

};

#endif
