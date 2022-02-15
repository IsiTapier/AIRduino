/*
  Display.h - Display-Library for Airduino.
*/

#ifndef DisplayVX_h
#define DisplayVX_h

#include "Arduino.h"
#include "Wire.h"
#include "../../util/Util.h"

#define ERROR (airCondition <= 0)
#define ERRORLAST (lastAirCondition <= 0)

class DisplayVX {

  public:
    static void setup();
    static void handleData();
    static void peepLoop();
    static State state;
    static State lastState;
    static String lastTime;
    static String time;
    static void calculateTime();
    static boolean recentPeepStatus;
    static bool isInitEnabled;
    
    static void drawPPMStraight(int x, int y, int size, int color, int datum, int backgroundColor);
    static void drawPPMStraight(int x, int y, int size, int color, int datum);

    static void drawRawPPMStraight(int x, int y, int size, int color, int datum, int backgroundColor);
    static void drawRawPPMStraight(int x, int y, int size, int color, int datum);

    static void drawTimer(int x, int y, int size, int color, int datum);
    static void showHelp(String txt, int x, int y, int size, int color, int datum, int duration, int startDelay, int currentGui);

    static void drawLoudspeaker();
    
  protected:
    static void drawDisplay();
    static void getData();
    static void generateData(int startPPM, int endPPM, int changePPM);
    static void checkState();
    static void writeInfo();
    static void drawEssentials();
    static void drawPPM();
    static void deleteTask(TaskHandle_t _task);
  


    static String lastPPM;
    static int lastPPMSize;

    static void drawTimer();
    //static void createLines();

    
    static int airCondition;
    static int lastAirCondition;
    static boolean blinkSwitch;
    static int statusLetters;
    static String statusInfo;
    
    static short seconds;
    static short minutes;
    static boolean start;
    static boolean drop;
    static short peep;
    static short peepCounter;

};

#endif
