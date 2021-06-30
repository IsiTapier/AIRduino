
#include "MenuGui.h"

void MenuGui::loop() {

} 

void MenuGui::initGui() {
    if(gui.equals(GUI_MENU)) {
        display.fillScreen(BLACK);
        dPrint("Men" + ue, MENU_ARROW_BACK_START_X - 5, 20, 2, GREY, 5);
        // display.pushImage(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, gear, WHITE);
        display.pushImage(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, homeDark, BLACK);
        display.pushImage(MENU_ARROW_RESET_START_X, MENU_ARROW_RESET_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, gear, WHITE);
        dPrint("CO2", DISPLAY_LENGTH/4, DISPLAY_HEIGHT*1/3, 2, WHITE, 4);
        
        dPrint("WETTER", DISPLAY_LENGTH/4, DISPLAY_HEIGHT*2/3, 2, WHITE, 4);

        dPrint("STOPPUHR", DISPLAY_LENGTH*2/4, DISPLAY_HEIGHT*1/3, 2, WHITE, 4);

        dPrint("TIMER", DISPLAY_LENGTH*2/4, DISPLAY_HEIGHT*2/3, 2, WHITE, 4);

        dPrint("r-Schüler", DISPLAY_LENGTH*3/4, DISPLAY_HEIGHT*1/3, 2, WHITE, 4);
        dPrint("L"+ae+"rmampel", DISPLAY_LENGTH*3/4, DISPLAY_HEIGHT*2/3, 2, WHITE, 4);
        
    }
}
