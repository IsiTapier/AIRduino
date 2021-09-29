
#include "RandomStudentGui.h"

#define TOP_MARGIN 45
#define LINE_THICKNESS 3
#define SIDE_MARGIN 50

int RandomStudentGui::maxRandomValue = 24;
int RandomStudentGui::lastMaxRandomValue = 24;
int RandomStudentGui::lastRandomValue = 0;

//false == random number; true == random student;
boolean calculateMode = false;


void RandomStudentGui::initGui() {
    if(gui.getValue() == RANDOM_STUDENT_GUI) {
        display.fillScreen(BLACK);

        Display::drawTopBar("Zufall");      
        if(client.connected()) calculateMode = true;
        drawSideBar(calculateMode);
        dPrint("RANDOM", DISPLAY_LENGTH/2, DISPLAY_HEIGHT-15, 3, COLOR_STATUS_RISK, 7);
    }  
}

void RandomStudentGui::drawSideBar(boolean mode) {

    //Lines right
    display.fillRect(DISPLAY_LENGTH-SIDE_MARGIN, TOP_MARGIN, LINE_THICKNESS, DISPLAY_HEIGHT-TOP_MARGIN, GREY);
    // display.fillRect(DISPLAY_LENGTH-SIDE_MARGIN, DISPLAY_HEIGHT/2, SIDE_MARGIN, LINE_THICKNESS, GREY);
/*     display.pushImage(DISPLAY_LENGTH-SIDE_MARGIN+9, 140-16, MENU_ICON_SIZE, MENU_ICON_SIZE, studentIcon);
    //Lines right
    display.fillRect(SIDE_MARGIN, TOP_MARGIN, LINE_THICKNESS, DISPLAY_HEIGHT-TOP_MARGIN, GREY);
    // display.fillRect(0, DISPLAY_HEIGHT/2, SIDE_MARGIN, LINE_THICKNESS, GREY);
    display.pushImage(DISPLAY_LENGTH-SIDE_MARGIN+9, 190-16, MENU_ICON_SIZE, MENU_ICON_SIZE, randomIcon); */
/*     if(mode) {
        display.fillRect()
    } */
    
    if(mode) {
        dPrint("Zahl", DISPLAY_LENGTH/2, TOP_MARGIN, 3, BLACK, 1);
        dPrint("Sch" +ue+ "ler", DISPLAY_LENGTH/2, TOP_MARGIN, 3, WHITE, 1);
    } else {
        dPrint("Schueler", DISPLAY_LENGTH/2, TOP_MARGIN, 3, BLACK, 1);
        dPrint("Sch" +ue+ "ler", DISPLAY_LENGTH/2, TOP_MARGIN, 3, WHITE, 1);
    }
}

void RandomStudentGui::drawArrows() {


    /* display.pushImage(DISPLAY_LENGTH - MENU_ICON_SIZE - 16, 80, MENU_ICON_SIZE, MENU_ICON_SIZE, arrowRight, WHITE);
    display.pushImage(16, 80, MENU_ICON_SIZE, MENU_ICON_SIZE, arrowLeft, WHITE);
    dPrint("1", DISPLAY_LENGTH/2-20, DISPLAY_HEIGHT*2/5, 7, WHITE, 5);
    display.fillRect(145, 95-2, 15, 4, GREY);
    
    dPrint("24", DISPLAY_LENGTH/2+20, DISPLAY_HEIGHT*2/5, 7, WHITE, 3);
    //display.fillRoundRect(60, DISPLAY_HEIGHT*3/5, 80, 80, 5, GREY);
    //display.fillRoundRect(DISPLAY_LENGTH-60-80, DISPLAY_HEIGHT*3/5, 80, 80, 5, GREY);
    dPrint("START", DISPLAY_LENGTH-60-40, DISPLAY_HEIGHT*3/5+40, 4, LIGHT_BLUE, 4); */
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



