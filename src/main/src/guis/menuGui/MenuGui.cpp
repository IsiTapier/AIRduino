
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
            Serial.println("UHR");
            gui.setValue(TIME_GUI);
            
        }
        if(p.isTouching(DISPLAY_LENGTH*6/11, DISPLAY_LENGTH, DISPLAY_HEIGHT*1/4-32, DISPLAY_HEIGHT*1/4+32)) {
            Serial.println("ZUFALL");    
            gui.setValue(RANDOM_STUDENT_GUI);
        }
        if(p.isTouching(DISPLAY_LENGTH*6/11, DISPLAY_LENGTH, DISPLAY_HEIGHT*2/4-32, DISPLAY_HEIGHT*2/4+32)) {
            Serial.println("LÄrmampel");
            gui.setValue(DECIBEL_GUI);
        }
        if(p.isTouching(DISPLAY_LENGTH*6/11, DISPLAY_LENGTH, DISPLAY_HEIGHT*3/4-32, DISPLAY_HEIGHT*3/4+32)) {           
            Serial.println(UE + "berblick");
            gui.setValue(OVERVIEW_GUI);
        }
        Serial.println(gui.getValue());
    }
}


void MenuGui::initGui() {
    if(gui.equals(GUI_MENU)) {
        display.fillScreen(BLACK);
        Display::drawTopBar("Men" + ue);
        if(WiFi.isConnected()) {
            display.pushImage(MENU_ARROW_RESET_START_X + 40, MENU_ARROW_RESET_START_Y, MENU_ICON_SIZE, MENU_ICON_SIZE, wlanIconOn, BLACK);
            if(client.connected()) {
                display.pushImage(MENU_ARROW_RESET_START_X + 80, MENU_ARROW_RESET_START_Y, MENU_ICON_SIZE, MENU_ICON_SIZE, mqttIcon, BLACK);
            }
        } else {
            display.pushImage(MENU_ARROW_RESET_START_X + 40, MENU_ARROW_RESET_START_Y, MENU_ICON_SIZE, MENU_ICON_SIZE, wlanIconOff, BLACK);
        }
        
        display.pushImage(15, DISPLAY_HEIGHT*1/4-8, MENU_ICON_SIZE, MENU_ICON_SIZE, co2Icon);
        dPrint("CO2", DISPLAY_LENGTH*2/11, DISPLAY_HEIGHT*1/4, 2, WHITE, 3);
        dPrint("Hauptfunktion", DISPLAY_LENGTH*2/11, DISPLAY_HEIGHT*1/4+16, 1, GREEN, 3);

        display.pushImage(15, DISPLAY_HEIGHT*2/4-8, MENU_ICON_SIZE, MENU_ICON_SIZE, weatherIcon);
        dPrint("WETTER", DISPLAY_LENGTH*2/11, DISPLAY_HEIGHT*2/4, 2, WHITE, 3);
        dPrint("von M" +ue+ "hlacker", DISPLAY_LENGTH*2/11, DISPLAY_HEIGHT*2/4+16, 1, GREY, 3);

        display.pushImage(15, DISPLAY_HEIGHT*3/4-8, MENU_ICON_SIZE, MENU_ICON_SIZE, clockIcon);
        dPrint("UHR", DISPLAY_LENGTH*2/11, DISPLAY_HEIGHT*3/4, 2, WHITE, 3);
        dPrint("Stoppuhr, Timer", DISPLAY_LENGTH*2/11, DISPLAY_HEIGHT*3/4+16, 1, GREY, 3);
        
        display.pushImage(DISPLAY_LENGTH/2, DISPLAY_HEIGHT*1/4-8, MENU_ICON_SIZE, MENU_ICON_SIZE, randomIcon);
        dPrint("ZUFALL", DISPLAY_LENGTH*7/11, DISPLAY_HEIGHT*1/4, 2, WHITE, 3);
        dPrint("Zufallssch"+ue+"ler", DISPLAY_LENGTH*7/11, DISPLAY_HEIGHT*1/4+16, 1, GREY, 3);
        
        display.pushImage(DISPLAY_LENGTH/2, DISPLAY_HEIGHT*2/4-8, MENU_ICON_SIZE, MENU_ICON_SIZE, trafficLightIcon);
        dPrint("L"+AE+"RMAMPEL", DISPLAY_LENGTH*7/11, DISPLAY_HEIGHT*2/4, 2, WHITE, 3);
        dPrint("Coming soon", DISPLAY_LENGTH*7/11, DISPLAY_HEIGHT*2/4+16, 1, GREY, 3);
        
        display.pushImage(DISPLAY_LENGTH/2, DISPLAY_HEIGHT*3/4-8, MENU_ICON_SIZE, MENU_ICON_SIZE, overviewIcon);
        dPrint(UE + "berblick", DISPLAY_LENGTH*7/11, DISPLAY_HEIGHT*3/4, 2, WHITE, 3);
        dPrint("alles in einem", DISPLAY_LENGTH*7/11, DISPLAY_HEIGHT*3/4+16, 1, COLOR_STATUS_RISK, 3);
    }
}
