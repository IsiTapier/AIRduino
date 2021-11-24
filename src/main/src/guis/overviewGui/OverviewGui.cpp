
#include "OverviewGui.h"
#define MARGIN 20

String OverviewGui::lastPPMString = "0";
String timerLastValue = "";

void OverviewGui::loop() {
    if(gui.getValue() != OVERVIEW_GUI) return;
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
    DecibelGui::drawTrafficLight(DecibelGui::trafficLightStage, 230, 195, 20);
    drawPPMTimer(MARGIN, 80, 4, WHITE);
    DisplayVX::drawPPMStraight(DISPLAY_LENGTH-MARGIN, 60, 5, DisplayVX::state.getColor(), 2);
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
        DisplayVX::drawPPMStraight(DISPLAY_LENGTH-MARGIN, 60, 5, DisplayVX::state.getColor(), 2);
        drawPPMTimer(MARGIN, 80, 4, WHITE);
        display.fillRect(MARGIN, 110, DISPLAY_LENGTH-(2*MARGIN), 4, GREY);
        

        dPrint("L" + AE + "RM", MARGIN, 195, 4, LIGHTGREY, 3);
        DecibelGui::lastTrafficDigit = -10;
                  
        if(TimerGui::isRunning) {
            dPrint("TIMER", MARGIN, 150, 4, LIGHTGREY, 3);
            dPrint(TimerGui::getDigitsOfMillis(TimerGui::goalMillis - millis()), DISPLAY_LENGTH-MARGIN, 130, 5, WHITE, 2);                  
        } else if(client.connected()) {       
            drawWeatherData(WeatherGui::temp, WeatherGui::weather, WeatherGui::humidity); 
        } else {
            //Stopwtch
            if(!StopwatchGui::isRunning) dPrint("00:00", DISPLAY_LENGTH-MARGIN, 150, 5, WHITE, 5);
            // StopwatchGui::drawStopwatch(DISPLAY_LENGTH-MARGIN, 150, 5);
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

void OverviewGui::drawWeatherData(String temp, String weather, String humidity) {
    /* dPrint(WeatherGui::temp + "C", 20, 50, 3, BLUE);
    WeatherGui::drawWeatherIcon(WeatherGui::forecastWeather3, 20, 120);
    dPrint(WeatherGui::humidity + "%", 20, 200, 2, WHITE); */
    display.fillRect(0, 115, DISPLAY_LENGTH, 50, BLACK);
    dPrint(temp + "C", MARGIN, 150, 5, LIGHTGREY, 3);
    WeatherGui::drawWeatherIcon(weather, 130, 120);
    dPrint(humidity + "%", 2*92+MARGIN, 150, 5, LIGHTGREY, 3);
 }