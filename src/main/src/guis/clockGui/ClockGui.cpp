
#include "ClockGui.h"

String lastTime = "00:00";
String lastDate = "01.01.";

void ClockGui::initGui() {
    if(gui.equals(CLOCK_GUI)) {
        if(client.connected()) {
            display.fillScreen(BLACK);
            Display::drawTopBar("Uhrzeit");
            loop();
        }
    }
}

void ClockGui::loop() {
    if(!gui.equals(CLOCK_GUI)) return;
    ClockGui::drawClock(DISPLAY_LENGTH/2, DISPLAY_HEIGHT/1.5-5, 8, 7, WHITE);
    ClockGui::drawDate(DISPLAY_LENGTH/2, DISPLAY_HEIGHT/1.5+5, 4, 1, ORANGE);
}

void ClockGui::drawDate(int x, int y, int size, int datum, int color) {
    if(lastDate != currentDate) {
        Serial.print("drawed Time: ");
        Serial.println(currentDate);
        dPrint(lastDate, x, y, size, BLACK, datum);
        dPrint(currentDate, x, y, size, color, datum);
        lastDate = currentDate;      
    }
}

void ClockGui::drawClock(int x, int y, int size, int datum, int color) {
    if(lastTime != currentTime) {
        Serial.print("drawed Time: ");
        Serial.println(currentTime);
        dPrint(lastTime, x, y, size, BLACK, datum);
        dPrint(currentTime, x, y, size, color, datum);
        lastTime = currentTime;
        
    }
}
