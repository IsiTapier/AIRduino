
#include "RandomStudentGui.h"

int RandomStudentGui::maxRandomValue = 24;
int RandomStudentGui::lastMaxRandomValue = 24;
int RandomStudentGui::lastRandomValue = 0;

void RandomStudentGui::initGui() {
    if(gui.getValue() == RANDOM_STUDENT_GUI) {
        Serial.println("init stopwatch");
        display.fillScreen(BLACK);
        drawArrows();
        display.fillRect(65, 215, 70, 5, LIGHTGREY);
        dPrint("Zufallszahl", MENU_ARROW_BACK_START_X - 5, 20, 2, GREY, 5);
        display.pushImage(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, homeDark, BLACK);
    }  
}

void RandomStudentGui::drawArrows() {
    display.pushImage(DISPLAY_LENGTH - MENU_ICON_SIZE - 16, 80, MENU_ICON_SIZE, MENU_ICON_SIZE, arrowRight, WHITE);
    display.pushImage(16, 80, MENU_ICON_SIZE, MENU_ICON_SIZE, arrowLeft, WHITE);
    dPrint("1", DISPLAY_LENGTH/2-20, DISPLAY_HEIGHT*2/5, 7, WHITE, 5);
    display.fillRect(145, 95-2, 15, 4, GREY);
    
    dPrint("24", DISPLAY_LENGTH/2+20, DISPLAY_HEIGHT*2/5, 7, WHITE, 3);
    //display.fillRoundRect(60, DISPLAY_HEIGHT*3/5, 80, 80, 5, GREY);
    //display.fillRoundRect(DISPLAY_LENGTH-60-80, DISPLAY_HEIGHT*3/5, 80, 80, 5, GREY);
    dPrint("START", DISPLAY_LENGTH-60-40, DISPLAY_HEIGHT*3/5+40, 4, LIGHT_BLUE, 4);
}
void RandomStudentGui::drawRandomNumber() {
    int number = random(1, maxRandomValue+1);
    dPrint(lastRandomValue, 137, DISPLAY_HEIGHT*3/5+16, 6, BLACK, 2);
    lastRandomValue = number;
    dPrint(number, 137, DISPLAY_HEIGHT*3/5+16, 6, LIME, 2);
    
}
void RandomStudentGui::drawMaxRandomValue() {
    dPrint(lastMaxRandomValue, DISPLAY_LENGTH/2+20, DISPLAY_HEIGHT*2/5, 7, BLACK, 3);
    dPrint(maxRandomValue, DISPLAY_LENGTH/2+20, DISPLAY_HEIGHT*2/5, 7, WHITE, 3);
}

void RandomStudentGui::changeMax(int difference) {
    lastMaxRandomValue = maxRandomValue;
    maxRandomValue += difference;
    if((maxRandomValue - difference) <= 0) {
        maxRandomValue = 0;
    } else if((maxRandomValue - difference) > 100) {
        maxRandomValue -= 100;
    }
    drawMaxRandomValue();
}



