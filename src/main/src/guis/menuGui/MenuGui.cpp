
#include "MenuGui.h"

void MenuGui::loop() {

} 

void MenuGui::handleTouch(TSPoint p) {
    if(gui.equals(GUI_MENU) && !mode.equals(MENU)) {
        if(p.isTouching(0, DISPLAY_LENGTH*5/11, DISPLAY_HEIGHT*1/4-32, DISPLAY_HEIGHT*1/4+32)) {
            Serial.println("CO2");
            gui.setValue(CO2_GUI);
        }
        if(p.isTouching(0, DISPLAY_LENGTH*5/11, DISPLAY_HEIGHT*2/4-32, DISPLAY_HEIGHT*2/4+32)) {
            Serial.println("WETTER");
            gui.setValue(WEATHER_GUI);
        }
        if(p.isTouching(0, DISPLAY_LENGTH*5/11, DISPLAY_HEIGHT*3/4-32, DISPLAY_HEIGHT*3/4+32)) {
            Serial.println("STOPPUHR");
            gui.setValue(STOPWATCH_GUI);
        }
        if(p.isTouching(DISPLAY_LENGTH*6/11, DISPLAY_LENGTH, DISPLAY_HEIGHT*1/4-32, DISPLAY_HEIGHT*1/4+32)) {
            Serial.println("TIMER");    
            gui.setValue(TIMER_GUI);
        }
        if(p.isTouching(DISPLAY_LENGTH*6/11, DISPLAY_LENGTH, DISPLAY_HEIGHT*2/4-32, DISPLAY_HEIGHT*2/4+32)) {
            Serial.println("ZUFALL");
            gui.setValue(RANDOM_STUDENT_GUI);
        }
        if(p.isTouching(DISPLAY_LENGTH*6/11, DISPLAY_LENGTH, DISPLAY_HEIGHT*3/4-32, DISPLAY_HEIGHT*3/4+32)) {
            Serial.println("LÄrmampel");
        }
    }
}


void MenuGui::initGui() {
    if(gui.equals(GUI_MENU)) {
        display.fillScreen(BLACK);
        dPrint("Men" + ue, MENU_ARROW_BACK_START_X - 5, 20, 2, GREY, 5);
        // display.pushImage(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, gear, WHITE);
        display.pushImage(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, homeDark, BLACK);
        display.pushImage(MENU_ARROW_RESET_START_X, MENU_ARROW_RESET_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, gear, WHITE);
        if(WiFi.isConnected()) {
            display.pushImage(MENU_ARROW_RESET_START_X + 40, MENU_ARROW_RESET_START_Y, MENU_ICON_SIZE, MENU_ICON_SIZE, wlanIconOn, BLACK);
            if(!client.connected()) {
                display.pushImage(MENU_ARROW_RESET_START_X + 80, MENU_ARROW_RESET_START_Y, MENU_ICON_SIZE, MENU_ICON_SIZE, mqttIcon, BLACK);
            }
        } else {
            display.pushImage(MENU_ARROW_RESET_START_X + 40, MENU_ARROW_RESET_START_Y, MENU_ICON_SIZE, MENU_ICON_SIZE, wlanIconOff, BLACK);
        }
        
        dPrint("CO2", DISPLAY_LENGTH*5/11, DISPLAY_HEIGHT*1/4, 2, WHITE, 5);
        dPrint("Hauptfunktion", DISPLAY_LENGTH*5/11, DISPLAY_HEIGHT*1/4+16, 1, GREEN, 5);

        dPrint("WETTER", DISPLAY_LENGTH*5/11, DISPLAY_HEIGHT*2/4, 2, WHITE, 5);
        dPrint("von M" +ue+ "hlacker", DISPLAY_LENGTH*5/11, DISPLAY_HEIGHT*2/4+16, 1, GREY, 5);

        dPrint("STOPPUHR", DISPLAY_LENGTH*5/11, DISPLAY_HEIGHT*3/4, 2, WHITE, 5);
        dPrint("", DISPLAY_LENGTH*5/11, DISPLAY_HEIGHT*3/4+16, 1, GREY, 5);
        
        dPrint("TIMER", DISPLAY_LENGTH*6/11, DISPLAY_HEIGHT*1/4, 2, WHITE, 3);
        dPrint("", DISPLAY_LENGTH*6/11, DISPLAY_HEIGHT*1/4+16, 1, COLOR_STATUS_ALARM, 3);
        
        dPrint("ZUFALL", DISPLAY_LENGTH*6/11, DISPLAY_HEIGHT*2/4, 2, WHITE, 3);
        dPrint("", DISPLAY_LENGTH*6/11, DISPLAY_HEIGHT*2/4+16, 1, COLOR_STATUS_ALARM, 3);
        
        dPrint("L"+AE+"RMAMPEL", DISPLAY_LENGTH*6/11, DISPLAY_HEIGHT*3/4, 2, WHITE, 3);
        dPrint("coming soon", DISPLAY_LENGTH*6/11, DISPLAY_HEIGHT*3/4+16, 1, COLOR_STATUS_ALARM, 3);
    }
}
