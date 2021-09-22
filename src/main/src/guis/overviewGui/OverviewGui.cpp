
#include "OverviewGui.h"
#define MARGIN 20

String OverviewGui::lastPPMString = "0";
String timerLastValue = "";

void OverviewGui::loop() {
    if(StopwatchGui::isRunning) StopwatchGui::drawStopwatch(DISPLAY_LENGTH-MARGIN, 150, 5);
    if(TimerGui::isRunning && TimerGui::goalMillis != 0) {
        //loop functions
        String digits = TimerGui::getDigitsOfMillis(TimerGui::goalMillis - millis());
        if(digits != timerLastValue) {
            dPrint(timerLastValue, DISPLAY_LENGTH-MARGIN, 130, 5, BLACK, 2);
            dPrint(digits, DISPLAY_LENGTH-MARGIN, 130, 5, WHITE, 2); 
            timerLastValue = digits;  
        }     
    }
    drawPPMTimer(MARGIN, 80, 4, WHITE);
    drawPPM();
}
 
void OverviewGui::initGui() {
    if(gui.equals(OVERVIEW_GUI)) {
        display.fillScreen(BLACK);
        
        Display::drawTopBar(UE + "berblick");
        display.pushImage(MENU_ARROW_RESET_START_X, MENU_ARROW_RESET_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, gear, WHITE);
        if(WiFi.isConnected()) {
            display.pushImage(MENU_ARROW_RESET_START_X + 40, MENU_ARROW_RESET_START_Y, MENU_ICON_SIZE, MENU_ICON_SIZE, wlanIconOn, BLACK);
            if(client.connected()) {
                display.pushImage(MENU_ARROW_RESET_START_X + 80, MENU_ARROW_RESET_START_Y, MENU_ICON_SIZE, MENU_ICON_SIZE, mqttIcon, BLACK);
            }
        } else {
            display.pushImage(MENU_ARROW_RESET_START_X + 40, MENU_ARROW_RESET_START_Y, MENU_ICON_SIZE, MENU_ICON_SIZE, wlanIconOff, BLACK);
        }        
        //CO2 Value
        dPrint("CO2", DISPLAY_LENGTH*3/4, 40, 2, LIGHTGREY, 1);
        drawPPM(true);
        drawPPMTimer(MARGIN, 80, 4, WHITE);
        display.fillRect(MARGIN, 110, DISPLAY_LENGTH-(2*MARGIN), 4, GREY);
        

        dPrint("L" + AE + "RM", MARGIN, 195, 4, LIGHTGREY, 3);
        for(int x = 1; x < 60; x += 10) {
            drawDecibleChart(x, 160, 175, 160-MARGIN, 40, 10, RED);
            delay(100);
        }

                  
         if(TimerGui::isRunning) {
            dPrint("TIMER", MARGIN, 150, 4, LIGHTGREY, 3);
            dPrint(TimerGui::getDigitsOfMillis(TimerGui::goalMillis - millis()), DISPLAY_LENGTH-MARGIN, 130, 5, WHITE, 2);                  
        } else if(client.connected()) {       
            drawWeatherData("-", "", "-"); 
        } else {
            //Stopwtch
            StopwatchGui::drawStopwatch(DISPLAY_LENGTH-MARGIN, 150, 5);
            dPrint("STOP", MARGIN, 150, 4, LIGHTGREY, 3);
        }
    }
}

void OverviewGui::drawPPMTimer(int x, int y, int size, int color) {
    DisplayVX::calculateTime();
    if(DisplayVX::lastTime != DisplayVX::time) {
        dPrint(DisplayVX::lastTime, x, y, size, BLACK, 3);
        dPrint(DisplayVX::time, x, y, size, color, 3);
        DisplayVX::lastTime = DisplayVX::time;
    }  
}

void OverviewGui::drawPPM(boolean atStart) {
    if(atStart == false) {
        if(DisplayVX::getLastPPMString() == lastPPMString) {
            return;
        }
    }
    Serial.println("drawPPM " + DisplayVX::getLastPPMString());

    dPrint(lastPPMString, DISPLAY_LENGTH-MARGIN, 60, 5, BLACK, 2);
    dPrint(DisplayVX::getPPMString(lastPPMString), DISPLAY_LENGTH-MARGIN, 60, 5, DisplayVX::state.getColor(), 2);
    lastPPMString = DisplayVX::getPPMString(lastPPMString);      
}

void OverviewGui::drawPPM() {
    drawPPM(false);
}

void OverviewGui::drawWeatherData(String temp, String weather, String humidity) {
    /* dPrint(WeatherGui::temp + "C", 20, 50, 3, BLUE);
    WeatherGui::drawWeatherIcon(WeatherGui::forecastWeather3, 20, 120);
    dPrint(WeatherGui::humidity + "%", 20, 200, 2, WHITE); */

    dPrint(temp + "C", MARGIN, 150, 5, LIGHTGREY, 3);
    WeatherGui::drawWeatherIcon(weather, 130, 110);
    dPrint(humidity + "%", 2*92+MARGIN, 150, 5, LIGHTGREY, 3);
 }

 void OverviewGui::drawDecibleChart(int value, int x, int y, int length, int height, int elements, int color) {
    int elementLength = length / elements;
    int amountOfElements = map(value, 1, 100, 1, elements);
    if(amountOfElements < 1) amountOfElements = 1;
    if(amountOfElements > elements) amountOfElements = elements;

    for(int z = 0; z < elements; z++) {
        if(z > amountOfElements) {
            display.fillRect(z*elementLength + x, y, 0.75*elementLength, height, GREY);
        } else {
            display.fillRect(z*elementLength + x, y, 0.75*elementLength, height, color);
        }
    }
 }