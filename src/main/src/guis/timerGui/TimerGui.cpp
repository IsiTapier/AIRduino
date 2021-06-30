
#include "TimerGui.h"

int TimerGui::goalMillis = 0;
int TimerGui::millisDifference = 0;
int TimerGui::millisAtPause = 0;
String TimerGui::lastTime = "";
String TimerGui::lastDifference = "";
boolean TimerGui::isRunning = false;


void TimerGui::loop() {
    if(gui.equals(TIMER_GUI)) {
        drawTimer();
    }
}

void TimerGui::initGui() {
    if(gui.getValue() == TIMER_GUI) {
        Serial.println("init timer");
        display.fillScreen(BLACK);
        dPrint("Timer", MENU_ARROW_BACK_START_X - 5, 20, 2, GREY, 5);
        drawButtons();
        dPrint("RESET", DISPLAY_LENGTH*1/4, DISPLAY_HEIGHT*5/6, 4, RED, 4);
        dPrint("00:00", DISPLAY_LENGTH - 20, 100, 9, BLACK, 5); 
        display.pushImage(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, homeDark, BLACK);
        drawAllArrows(GREY);
    }  
}

void TimerGui::toggleTimer() {
    if(isRunning) {
        stopTimer();
    } else {
        if(millisDifference > 0)
            startTimer();
    }
    drawButtons();
}

void TimerGui::startTimer() {
    isRunning = true;
    if(millisAtPause == 0) {
        goalMillis = millis() + millisDifference;
    } else {
        goalMillis = goalMillis + millisAtPause;
    }
    Serial.println("start timer");
    millisAtPause = 0;
}

void TimerGui::stopTimer() {
    isRunning = false;
    goalMillis = 0;
    millisAtPause = millis();
    Serial.println("stop timer");
}

void TimerGui::resetTimer() {
    millisDifference = 0;
    goalMillis = 0;
    millisAtPause = 0;
    isRunning = false;
    Serial.println("reset timer");
}

void TimerGui::drawTimer() {
    if(goalMillis == 0) {
        drawDigits(millisDifference); 
    } else {
        drawDigits(goalMillis - millis());
    }
}

void TimerGui::drawAllArrows(int color) {   
    drawArrowUp(53, 56, 40, color);
    drawArrowUp(207, 56, 40, color);

    drawArrowDown(53, 135, 40, color);
    drawArrowDown(207, 135, 40, color);
}

void TimerGui::drawArrowDown(int x, int y, int size, int color) {
    display.fillTriangle(x, y, x + size, y, x + size/2, y + size*2/3, color);
}

void TimerGui::drawArrowUp(int x, int y, int size, int color) {
    display.fillTriangle(x, y, x + size, y, x + size/2, y - size*2/3, color);
}

void TimerGui::updateDigit(int digit, int difference) {
    if(!isRunning) {
        if(digit == 1) { //Minutes
            millisDifference += difference * 60 * 1000;
        }
        if(digit == 2) { //seconds
            millisDifference += difference * 1000;
        }
        if(millisDifference <= 0) {
            millisDifference = 0;
        }
        drawDigits(millisDifference);
        Serial.println(millisDifference);
    } 
}

void TimerGui::drawDigits(int totalSeconds) {
    totalSeconds = totalSeconds / 1000;
    int minutes = round(totalSeconds/60);
    int seconds = totalSeconds - (minutes * 60);

    String digits;
    if(minutes < 10)
        digits = digits + "0";
    digits += minutes;
    digits += ":";

    if(seconds < 10)
        digits = digits + "0";
    digits += seconds;
    
    if(!isRunning) {
        dPrint(lastDifference, DISPLAY_LENGTH - 20, 80, 9, BLACK, 5);
        dPrint(digits, DISPLAY_LENGTH - 20, 80, 9, WHITE, 5);
        lastDifference = digits;
    } else {
        if(lastTime != digits) {
            if(lastTime != "") {
                if(digits[1] != lastTime[1]) {
                    dPrint(digits[1] + ":"+ digits[2]+ digits[3], DISPLAY_LENGTH - 20, 100, 9, BLACK, 5); 
                } else if(digits[2] != lastTime[2]) {
                    dPrint(digits[2] + digits[3], DISPLAY_LENGTH - 20, 100, 9, BLACK, 5); 
                } else if(digits[3] != lastTime[3]) {
                    dPrint(digits[3], DISPLAY_LENGTH - 20, 100, 9, BLACK, 5); 
                } else {
                    dPrint(digits, DISPLAY_LENGTH - 20, 100, 9, BLACK, 5); 
                }
            }               
        }  
        lastTime = digits;
    }
    
    if((seconds == 0) && (minutes == 0)) {
        resetTimer();
    }
}

void TimerGui::drawButtons() {
    if(isRunning) {
        drawAllArrows(BLACK);
        dPrint("START", DISPLAY_LENGTH*3/4, DISPLAY_HEIGHT*5/6, 4, BLACK, 4); 
        dPrint("STOP", DISPLAY_LENGTH*3/4, DISPLAY_HEIGHT*5/6, 4, ORANGE, 4);
    } else {
        drawAllArrows(GREY);       
        dPrint("STOP", DISPLAY_LENGTH*3/4, DISPLAY_HEIGHT*5/6, 4, BLACK, 4);
        dPrint("START", DISPLAY_LENGTH*3/4, DISPLAY_HEIGHT*5/6, 4, ORANGE, 4);
    }
}