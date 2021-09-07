
#include "OverviewGui.h"
#define MARGIN 20

String OverviewGui::lastPPMString = "0";

void OverviewGui::loop() {
    StopwatchGui::drawStopwatch(DISPLAY_LENGTH-MARGIN, 130, 5);
    drawPPM();
} 

void OverviewGui::initGui() {
    if(gui.equals(OVERVIEW_GUI)) {
        display.fillScreen(BLACK);

        //draw Basic Strings
        dPrint("CO2", MARGIN, 60, 5, GREY);
        dPrint("STOP", MARGIN, 110, 5, GREY);
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
        drawPPM(true);
        StopwatchGui::drawStopwatch(DISPLAY_LENGTH-MARGIN, 130, 5);

        // if(client.connected()) {           
            drawWeatherData();           
        // }
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

void OverviewGui::drawWeatherData() {
    /* dPrint(WeatherGui::temp + "C", 20, 50, 3, BLUE);
    WeatherGui::drawWeatherIcon(WeatherGui::forecastWeather3, 20, 120);
    dPrint(WeatherGui::humidity + "%", 20, 200, 2, WHITE); */
    dPrint("30C", MARGIN, 160, 5, LIGHTGREY);
    WeatherGui::drawWeatherIcon("Rain", 130, 160);
    dPrint("60%", 2*92+MARGIN, 160, 5, LIGHTGREY);
 }