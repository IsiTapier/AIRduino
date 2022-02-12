
#include "TimeGui.h"

const String clockTitle = "Uhrzeit";
const String timerTitle = "Timer";
const String stopwatchTitle = "Stoppuhr";


int stopWatchLocX = DISPLAY_LENGTH/2;
int stopWatchLocY = DISPLAY_HEIGHT*1/4;
int timerLocX = DISPLAY_LENGTH/2;
int timerLocY = DISPLAY_HEIGHT*2/4;
int clockLocX = DISPLAY_LENGTH/2;
int clockLocY = 3/4*DISPLAY_HEIGHT;
int clockColor = 0x07ED;
int stripeColor = GREY;

void TimeGui::initGui() {
    if(gui.equals(TIME_GUI)) {
        display.fillScreen(BLACK);
        Display::drawTopBar("Uhr");
        Serial.print("timer loc y");
        Serial.println(DISPLAY_LENGTH/4);

        display.pushImage(DISPLAY_LENGTH*1/6 - 32, DISPLAY_HEIGHT/2-32, 64, 64, stopwatchIcon);
        dPrint(stopwatchTitle, DISPLAY_LENGTH*1/6, DISPLAY_HEIGHT/2 + 50, 2, GREY, 4);

        display.pushImage(DISPLAY_LENGTH*3/6 - 32, DISPLAY_HEIGHT/2-32, 64, 64, timerIcon);
        dPrint(timerTitle, DISPLAY_LENGTH*3/6, DISPLAY_HEIGHT/2 + 50, 2, GREY, 4);
        
        display.pushImage(DISPLAY_LENGTH*5/6 - 32, DISPLAY_HEIGHT/2-32, 64, 64, clockIconBig);
        dPrint(clockTitle, DISPLAY_LENGTH*5/6, DISPLAY_HEIGHT/2 + 50, 2, GREY, 4);

        display.fillRect(DISPLAY_LENGTH*1/3, 65, 4, DISPLAY_HEIGHT-60*2, GREY);
        display.fillRect(DISPLAY_LENGTH*2/3, 65, 4, DISPLAY_HEIGHT-60*2, GREY);

    }
}

void TimeGui::handleTouch(TSPoint p) {
    if(gui.equals(TIME_GUI)) {
        //UHR
        Serial.println("Handle Touch Time Gui");
        if(p.isTouching(0, DISPLAY_LENGTH*1/3, DISPLAY_HEIGHT*1/4, DISPLAY_HEIGHT)) {
            gui.setValue(STOPWATCH_GUI);
        } else if(p.isTouching(DISPLAY_LENGTH*1/3, DISPLAY_LENGTH*2/3, DISPLAY_HEIGHT*1/4, DISPLAY_HEIGHT)) {
            gui.setValue(TIMER_GUI);
        } else if(p.isTouching(DISPLAY_LENGTH*2/3, DISPLAY_LENGTH, DISPLAY_HEIGHT*1/4, DISPLAY_HEIGHT)) {
            if(isClientConnected) {
                gui.setValue(CLOCK_GUI);
            } else {
                dPrint("error", DISPLAY_LENGTH*5/6, DISPLAY_HEIGHT/2 + 70, 2, ORANGE, 4);
                dPrint("Keine Verbindung zum Server...", DISPLAY_LENGTH/2, DISPLAY_HEIGHT - 20, 1, RED, BC_DATUM);
            } 
        }
    }
}




