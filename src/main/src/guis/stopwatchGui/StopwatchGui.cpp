
#include "StopwatchGui.h"

String StopwatchGui::lastTime = "";
int StopwatchGui::startMillis = 0;
int StopwatchGui::millisAtPause = 0;
boolean StopwatchGui::isRunning = false;

void StopwatchGui::loop() {  
    drawStopwatch();   
}

void StopwatchGui::initGui() {
    if(gui.getValue() == STOPWATCH_GUI) {
        Serial.println("init stopwatch");
        display.fillScreen(BLACK);
        dPrint("RESET", DISPLAY_LENGTH*1/4, DISPLAY_HEIGHT*3/4, 4, ORANGE, 4);
        dPrint("Stoppuhr", MENU_ARROW_BACK_START_X - 5, 20, 2, GREY, 5);
        display.pushImage(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, homeDark, BLACK);
        drawStopwatch();
        drawButtons();
    }  
}

void StopwatchGui::startStopwatch() {
    if(startMillis == 0) {
        startMillis = millis(); 
        Serial.println("started stopwatch");
    } else {
        Serial.println("resumed stopwatch");
        Serial.println(startMillis);
        startMillis += millis() - millisAtPause;
        Serial.println(startMillis);
        millisAtPause = 0;
    }
    isRunning = true;  
}
void StopwatchGui::stopStopwatch() {
    isRunning = false;
    millisAtPause = millis();

    Serial.println("Stopwatch paused");
    }

void StopwatchGui::resetStopwatch() {
    isRunning = false;
    millisAtPause = 0;
    startMillis = 0;
    }

void StopwatchGui::drawButtons() {
    if(isRunning) {
        dPrint("START", DISPLAY_LENGTH*3/4, DISPLAY_HEIGHT*3/4, 4, BLACK, 4);
        dPrint("STOP", DISPLAY_LENGTH*3/4, DISPLAY_HEIGHT*3/4, 4, LIME, 4);   

    } else {
        dPrint("STOP", DISPLAY_LENGTH*3/4, DISPLAY_HEIGHT*3/4, 4, BLACK, 4);
        dPrint("START", DISPLAY_LENGTH*3/4, DISPLAY_HEIGHT*3/4, 4, LIME, 4); 
        
    }
}

void StopwatchGui::toggleStopwatch() {
    if(isRunning) {
        stopStopwatch();
    } else {
        startStopwatch();
    }
    drawIcon();
    drawButtons(); 
}

void StopwatchGui::drawStopwatch() {
    if(isRunning) {
        drawDigits(millis() - startMillis);    
    } else {        
        // dPrint(lastTime, 160, 80, 9, BLACK, 4);
        if(millisAtPause)
            drawDigits(0);
        else 
            drawDigits(millisAtPause - startMillis);
    }  
}

void StopwatchGui::drawLines() {
    display.fillRoundRect(163, 120, 3, 400, 3, LIGHTGREY);
    display.fillRoundRect(0, 123, DISPLAY_LENGTH, 3, 3, LIGHTGREY);

    display.fillRoundRect(153, 120, 10, 400, 3, GREY);
    display.fillRoundRect(0, 113, DISPLAY_LENGTH, 10, 3, GREY);
}

void StopwatchGui::drawDigits(long totalSeconds) {
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
    if(lastTime != digits) {
        if(lastTime != "") dPrint(lastTime, DISPLAY_LENGTH - 20, 80, 9, BLACK, 5);
        dPrint(digits, DISPLAY_LENGTH - 20, 80, 9, WHITE, 5);       
    }  
    lastTime = digits; 
}

void StopwatchGui::drawIcon() {
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
