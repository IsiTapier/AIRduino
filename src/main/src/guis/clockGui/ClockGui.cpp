
#include "ClockGui.h"

String lastTime = "00:00";

void ClockGui::initGui() {
    if(gui.equals(GUI_MENU)) {
        if(client.connected()) {
            display.fillScreen(BLACK);
            Display::drawTopBar("Uhrzeit");

        }
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
