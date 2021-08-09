
#include "TimerGui.h"

int TimerGui::goalMillis = 0;
int TimerGui::millisDifference = 0;
int TimerGui::millisToGoal = 0;
String TimerGui::lastTime = "00:00";
String TimerGui::lastDifference = "00:00";
boolean TimerGui::isRunning = false;
int TimerGui::peepCount = 0;



void TimerGui::loop() {
    if(gui.equals(TIMER_GUI)) {
        if(isRunning) {
            /* Serial.print("-on-");
            Serial.println((goalMillis - millis())); */
        }        
        if(isRunning && 500 >= (goalMillis - millis())) {
            resetTimer();
        }
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

        display.fillRect(0, 64, DISPLAY_LENGTH, 64, BLACK);
        drawTimer();
        drawIcon();
        if(!isRunning) {
            drawDigits(millisDifference);
        }      
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
    drawIcon();
}

void TimerGui::startTimer() {
    isRunning = true;
    if(millisToGoal == 0) {
        lastTime = getDigitsOfMillis(millisDifference);
        goalMillis = millis() + millisDifference;
    } else {
        goalMillis = millis() + millisToGoal;
    }
    Serial.println("start timer");
    millisToGoal = 0;
}

void TimerGui::stopTimer() {
    isRunning = false;
    millisToGoal = goalMillis - millis();
    Serial.println("stop timer");
}

void TimerGui::resetTimer() {
    Serial.println("RESET");
    display.fillRect(0, 64, DISPLAY_LENGTH, 90, BLACK);
    if(isRunning) {        
        goalMillis = 0;
        millisToGoal = 0;
        isRunning = false;
        Serial.println("reset timer");
        drawAllArrows(GREY);
        peepCount = 1;
        return;
    } else if(!isRunning && (millisDifference != 0)) {
        millisDifference = 0;      
        return;
    } 
    drawButtons();
    drawDigits(millisDifference);
    drawIcon(); 
}

void TimerGui::drawTimer() {
    if(goalMillis != 0) {
        if(isRunning) {
            drawDigits(goalMillis - millis());
            // Serial.println("drawed Digit " + getDigitsOfMillis(goalMillis - millis())); 
        }       
    }
}

void TimerGui::drawAllArrows(int color) {   
    drawArrowUp(53, 56, 40, color);
    drawArrowUp(220, 56, 40, color);

    drawArrowDown(53, 135, 40, color);
    drawArrowDown(220, 135, 40, color);
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
        if(millisDifference < 0) {
            millisDifference = 99*60*1000 + millisDifference;
        }
        if(millisDifference > 99*60*1000) {
            millisDifference = millisDifference - 99*60*1000;
        }
        drawDigits(millisDifference);
        // Serial.println(millisDifference);
    } 
}

String TimerGui::getDigitsOfMillis(int _millis) {
    _millis = _millis / 1000;
    int minutes = round(_millis/60);
    int seconds = _millis - (minutes * 60);
    
    String digits;
    if(minutes < 10)
        digits = digits + "0";
    digits += minutes;
    digits += ":";

    if(seconds < 10)
        digits = digits + "0";
    digits += seconds;
    return digits;
}

void TimerGui::drawDigits(int _millis) {
    String digits = getDigitsOfMillis(_millis);
    if(!isRunning) {
        dPrint(lastDifference, DISPLAY_LENGTH - 20, 100, 9, BLACK, 5);
        dPrint(digits, DISPLAY_LENGTH - 20, 100, 9, WHITE, 5);
        lastDifference = digits;
    } else {
        if(lastTime != digits) {
            if(lastTime != "00:00") {                
                dPrint(lastTime, DISPLAY_LENGTH - 20, 100, 9, BLACK, 5);
                dPrint(digits, DISPLAY_LENGTH - 20, 100, 9, WHITE, 5);               
            }               
        }  
        lastTime = digits;
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

void TimerGui::peep() {
    if(peepCount != 0) {
        if(peepCount%2 == 1) {
            dPrint(".", 10, 10, 2, WHITE);          
        } else {
            dPrint(".", 10, 10, 2, BLACK);         
        }
        peepCount--;
    }
    
}

void TimerGui::drawIcon() {
    if(isRunning) {     
        display.fillRect(150, 120, 8, 25, BLACK);
        display.fillRect(166, 120, 8, 25, BLACK);
        display.fillTriangle(153, 120, 153, 140, 173, 130, LIGHTGREY);  

    } else {
        display.fillTriangle(153, 120, 153, 140, 173, 130, BLACK); 
        display.fillRect(150, 120, 8, 25, LIGHTGREY);
        display.fillRect(166, 120, 8, 25, LIGHTGREY);        
    }   
}