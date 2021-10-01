
#include "RandomStudentGui.h"

#define TOP_MARGIN 45
#define LINE_THICKNESS 5
#define SIDE_MARGIN 50
#define LINE_MARGIN 10
#define MIDDLE_LINE_MARGIN 5
#define UNDERLINE_MARGIN 7
#define UNDERLINE_OFFSET 5
#define ROUNDING 2
#define TOUCH_FIELD_INCREASE 10
#define SPIN_TEXT_SIZE 4
#define ROLL_LENGTH 20

#define NO_REAPEAT true

int RandomStudentGui::minRandomValue = 1;
int RandomStudentGui::maxRandomValue = 24;
int RandomStudentGui::lastMaxRandomValue = 24;
int RandomStudentGui::lastRandomValue = 0;

//false == random number; true == random student;
boolean calculateMode = false;
boolean minValueSelect = true;
boolean lastTouching = 0;
unsigned long lastTouch = 0;
std::vector<String> testclass = {"Jan", "Tim", "Willibon", "Isajah", "Jonas", "Josua", "Cindy", "Marit", "Helena", "Lotta", "Hannah", "Rozerin", "Janice", "Maja", "Rahel", "Nele"};
std::vector<String> temparray;

#define LONG_PRESS (lastTouching)

void RandomStudentGui::initGui()
{
    if (gui.getValue() == RANDOM_STUDENT_GUI)
    {
        display.fillScreen(BLACK);
        saveClassList("Tim Ramsch, Leon Kugel, Lea Ulti, Baba Ralf Erich");
        // if(client.connected()) calculateMode = true;
        temparray = testclass;
        drawSideBar();
        drawGui();
    }  

}

void RandomStudentGui::drawSideBar()
{
    //Lines right
    display.fillRoundRect(DISPLAY_LENGTH - SIDE_MARGIN, TOP_MARGIN, LINE_THICKNESS, DISPLAY_HEIGHT - TOP_MARGIN - LINE_MARGIN, ROUNDING, GREY);
    display.fillRoundRect(DISPLAY_LENGTH - SIDE_MARGIN + MIDDLE_LINE_MARGIN + LINE_THICKNESS, TOP_MARGIN + (DISPLAY_HEIGHT - TOP_MARGIN - LINE_MARGIN - LINE_THICKNESS / 2.) / 2, SIDE_MARGIN - LINE_THICKNESS - 2 * MIDDLE_LINE_MARGIN, LINE_THICKNESS, ROUNDING, GREY);
    display.pushImage(DISPLAY_LENGTH - SIDE_MARGIN + MIDDLE_LINE_MARGIN + LINE_THICKNESS + (SIDE_MARGIN - 2 * MIDDLE_LINE_MARGIN - LINE_THICKNESS - MENU_ICON_LENGTH) / 2, TOP_MARGIN + (DISPLAY_HEIGHT - TOP_MARGIN - LINE_MARGIN - LINE_THICKNESS - 2 * MENU_ICON_HEIGHT) / 4, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, studentIcon, BLACK);
    display.pushImage(DISPLAY_LENGTH - SIDE_MARGIN + MIDDLE_LINE_MARGIN + LINE_THICKNESS + (SIDE_MARGIN - 2 * MIDDLE_LINE_MARGIN - LINE_THICKNESS - MENU_ICON_LENGTH) / 2, DISPLAY_HEIGHT - LINE_MARGIN - MENU_ICON_HEIGHT - (DISPLAY_HEIGHT - TOP_MARGIN - LINE_MARGIN - LINE_THICKNESS - 2 * MENU_ICON_HEIGHT) / 4, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, singleDiceIcon, BLACK);
    display.fillRoundRect(SIDE_MARGIN - LINE_THICKNESS, TOP_MARGIN, LINE_THICKNESS, DISPLAY_HEIGHT - TOP_MARGIN - LINE_MARGIN, ROUNDING, GREY);
    display.fillRoundRect(MIDDLE_LINE_MARGIN, TOP_MARGIN + (DISPLAY_HEIGHT - TOP_MARGIN - LINE_MARGIN - LINE_THICKNESS / 2.) / 2, SIDE_MARGIN - LINE_THICKNESS - 2 * MIDDLE_LINE_MARGIN, LINE_THICKNESS, ROUNDING, GREY);
    display.pushImage(MIDDLE_LINE_MARGIN + (SIDE_MARGIN - 2 * MIDDLE_LINE_MARGIN - LINE_THICKNESS - MENU_ICON_LENGTH) / 2, TOP_MARGIN + (DISPLAY_HEIGHT - TOP_MARGIN - LINE_MARGIN - LINE_THICKNESS - 2 * MENU_ICON_HEIGHT) / 4, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, arrowUpIcon, BLACK);
    display.pushImage(MIDDLE_LINE_MARGIN + (SIDE_MARGIN - 2 * MIDDLE_LINE_MARGIN - LINE_THICKNESS - MENU_ICON_LENGTH) / 2, DISPLAY_HEIGHT - LINE_MARGIN - MENU_ICON_HEIGHT - (DISPLAY_HEIGHT - TOP_MARGIN - LINE_MARGIN - LINE_THICKNESS - 2 * MENU_ICON_HEIGHT) / 4, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, arrowDownIcon, BLACK);
    // display.fillRect(DISPLAY_LENGTH-SIDE_MARGIN, DISPLAY_HEIGHT/2, SIDE_MARGIN, LINE_THICKNESS, GREY);
    /*     display.pushImage(DISPLAY_LENGTH-SIDE_MARGIN+9, 140-16, MENU_ICON_SIZE, MENU_ICON_SIZE, studentIcon);
    //Lines right
    display.fillRect(SIDE_MARGIN, TOP_MARGIN, LINE_THICKNESS, DISPLAY_HEIGHT-TOP_MARGIN, GREY);
    // display.fillRect(0, DISPLAY_HEIGHT/2, SIDE_MARGIN, LINE_THICKNESS, GREY);
    display.pushImage(DISPLAY_LENGTH-SIDE_MARGIN+9, 190-16, MENU_ICON_SIZE, MENU_ICON_SIZE, randomIcon); */
    /*     if(mode) {
        display.fillRect()
    } */
    /*
    if(mode) {
        dPrint("Zahl", DISPLAY_LENGTH/2, TOP_MARGIN, 3, BLACK, 1);
        dPrint("Sch" +ue+ "ler", DISPLAY_LENGTH/2, TOP_MARGIN, 3, WHITE, 1);
    } else {
        dPrint("Schueler", DISPLAY_LENGTH/2, TOP_MARGIN, 3, BLACK, 1);
        dPrint("Sch" +ue+ "ler", DISPLAY_LENGTH/2, TOP_MARGIN, 3, WHITE, 1);
    }*/
}

void RandomStudentGui::drawGui(bool mode)
{
    Display::drawTopBar(String(calculateMode ? "Sch" + ue + "ler" : "Zahl") + " - Zufall", String(!calculateMode ? "Sch" + ue + "ler" : "Zahl") + " - Zufall");
    display.fillRect(SIDE_MARGIN, TOP_MARGIN, DISPLAY_LENGTH - 2 * SIDE_MARGIN, DISPLAY_HEIGHT - TOP_MARGIN, BLACK);
    dPrint("RANDOM", DISPLAY_LENGTH / 2, DISPLAY_HEIGHT - 15, 3, COLOR_STATUS_RISK, 7);
    if(!mode) {
         if (calculateMode) {
            dPrint("Zahl", DISPLAY_LENGTH / 2, TOP_MARGIN + 10, 3, BLACK, 1);
            dPrint("Sch" + ue + "ler", DISPLAY_LENGTH / 2, TOP_MARGIN + 10, 3, WHITE, 1);
        } else {
            dPrint("Sch" + ue + "ler", DISPLAY_LENGTH / 2, TOP_MARGIN + 10, 3, BLACK, 1);
            dPrint("Zahl", DISPLAY_LENGTH / 2, TOP_MARGIN + 10, 3, WHITE, 1);
        }
        display.fillRoundRect(DISPLAY_LENGTH - SIDE_MARGIN + UNDERLINE_MARGIN + LINE_THICKNESS, ((DISPLAY_HEIGHT - TOP_MARGIN - LINE_MARGIN - LINE_THICKNESS - 2 * MENU_ICON_HEIGHT) / 4) * (!calculateMode ? 1 : -1) + (!calculateMode ? TOP_MARGIN : DISPLAY_HEIGHT - LINE_MARGIN - MENU_ICON_HEIGHT) + MENU_ICON_HEIGHT + UNDERLINE_OFFSET, SIDE_MARGIN - LINE_THICKNESS - 2 * UNDERLINE_MARGIN, LINE_THICKNESS, ROUNDING, BLACK);
        display.fillRoundRect(DISPLAY_LENGTH - SIDE_MARGIN + UNDERLINE_MARGIN + LINE_THICKNESS, ((DISPLAY_HEIGHT - TOP_MARGIN - LINE_MARGIN - LINE_THICKNESS - 2 * MENU_ICON_HEIGHT) / 4) * (calculateMode ? 1 : -1) + (calculateMode ? TOP_MARGIN : DISPLAY_HEIGHT - LINE_MARGIN - MENU_ICON_HEIGHT) + MENU_ICON_HEIGHT + UNDERLINE_OFFSET, SIDE_MARGIN - LINE_THICKNESS - 2 * UNDERLINE_MARGIN, LINE_THICKNESS, ROUNDING, LIGHTGREY);
        if(!calculateMode) {
            dPrint((minRandomValue < 10 ? "0" : "") + String(minRandomValue) + (maxRandomValue < 10 ? "-0" : "-") + String(maxRandomValue), DISPLAY_LENGTH / 2 + 3, TOP_MARGIN + LINE_MARGIN + (DISPLAY_HEIGHT - TOP_MARGIN - LINE_MARGIN) / 2 - 6, 6, LIGHTGREY, CC_DATUM, BLACK, "  -  ");
            // dPrint("-", DISPLAY_LENGTH/2+3, TOP_MARGIN+LINE_MARGIN+(DISPLAY_HEIGHT-TOP_MARGIN-LINE_MARGIN)/2-3, 6, GREY, CC_DATUM, GREY);
            display.fillRoundRect(DISPLAY_LENGTH / 2 - 2 * LETTER_LENGTH, TOP_MARGIN + LINE_MARGIN + (DISPLAY_HEIGHT - TOP_MARGIN - LINE_MARGIN - LINE_THICKNESS) / 2 - 10, 4 * LETTER_LENGTH, LINE_THICKNESS, ROUNDING, GREY);
            display.fillRoundRect(DISPLAY_LENGTH / 2 + (!minValueSelect ? 3 : 3 + 3 * 6 * LETTER_LENGTH) - 5 * 3 * LETTER_LENGTH, TOP_MARGIN + LINE_MARGIN + (DISPLAY_HEIGHT - TOP_MARGIN - LINE_MARGIN) / 2 - 3 + 3 * LETTER_HEIGHT, 11 * LETTER_LENGTH, LINE_THICKNESS, ROUNDING, BLACK);
            display.fillRoundRect(DISPLAY_LENGTH / 2 + (minValueSelect ? 3 : 3 + 3 * 6 * LETTER_LENGTH) - 5 * 3 * LETTER_LENGTH, TOP_MARGIN + LINE_MARGIN + (DISPLAY_HEIGHT - TOP_MARGIN - LINE_MARGIN) / 2 - 3 + 3 * LETTER_HEIGHT, 11 * LETTER_LENGTH, LINE_THICKNESS, ROUNDING, WHITE);
        } else {

        }
    } else {
        ledcDetachPin(PIEZO);
        if(temparray.size() == 0)
            temparray = testclass;
        int minValue = calculateMode?0:minRandomValue;
        int maxValue = calculateMode?15:maxRandomValue;
        int randomValue = random(minValue, maxValue+1);
        for (int i = randomValue+max(minValue, maxValue-2-ROLL_LENGTH); i < randomValue+maxValue-2; i++){
            for (int j = 0.5*SPIN_TEXT_SIZE*LETTER_HEIGHT; j <= 1.5*SPIN_TEXT_SIZE*LETTER_HEIGHT; j++) {
                dPrint(calculateMode?testclass[(i+(j>SPIN_TEXT_SIZE*LETTER_HEIGHT?3:2))%(maxValue+1)]:String(minValue+(i+(j>SPIN_TEXT_SIZE*LETTER_HEIGHT?3:2))%(maxValue-minValue+1)), DISPLAY_LENGTH/2, DISPLAY_HEIGHT/2 + (j-1)%(SPIN_TEXT_SIZE*LETTER_HEIGHT) - 1.5*SPIN_TEXT_SIZE*LETTER_HEIGHT, SPIN_TEXT_SIZE, BLACK, CC_DATUM);
                dPrint(calculateMode?testclass[(i+(j>SPIN_TEXT_SIZE*LETTER_HEIGHT?2:1))%(maxValue+1)]:String(minValue+(i+(j>SPIN_TEXT_SIZE*LETTER_HEIGHT?2:1))%(maxValue-minValue+1)), DISPLAY_LENGTH/2, DISPLAY_HEIGHT/2 + (j-1)%(SPIN_TEXT_SIZE*LETTER_HEIGHT) - 0.5*SPIN_TEXT_SIZE*LETTER_HEIGHT, SPIN_TEXT_SIZE, BLACK, CC_DATUM);
                dPrint(calculateMode?testclass[(i+(j>SPIN_TEXT_SIZE*LETTER_HEIGHT?1:0))%(maxValue+1)]:String(minValue+(i+(j>SPIN_TEXT_SIZE*LETTER_HEIGHT?1:0))%(maxValue-minValue+1)), DISPLAY_LENGTH/2, DISPLAY_HEIGHT/2 + (j-1)%(SPIN_TEXT_SIZE*LETTER_HEIGHT) + 0.5*SPIN_TEXT_SIZE*LETTER_HEIGHT, SPIN_TEXT_SIZE, BLACK, CC_DATUM);
                dPrint(calculateMode?testclass[(i+(j>=SPIN_TEXT_SIZE*LETTER_HEIGHT?3:2))%(maxValue+1)]:String(minValue+(i+(j>=SPIN_TEXT_SIZE*LETTER_HEIGHT?3:2))%(maxValue-minValue+1)), DISPLAY_LENGTH/2, DISPLAY_HEIGHT/2 + j%(SPIN_TEXT_SIZE*LETTER_HEIGHT) - 1.5*SPIN_TEXT_SIZE*LETTER_HEIGHT, SPIN_TEXT_SIZE, WHITE, CC_DATUM);
                dPrint(calculateMode?testclass[(i+(j>=SPIN_TEXT_SIZE*LETTER_HEIGHT?2:1))%(maxValue+1)]:String(minValue+(i+(j>=SPIN_TEXT_SIZE*LETTER_HEIGHT?2:1))%(maxValue-minValue+1)), DISPLAY_LENGTH/2, DISPLAY_HEIGHT/2 + j%(SPIN_TEXT_SIZE*LETTER_HEIGHT) - 0.5*SPIN_TEXT_SIZE*LETTER_HEIGHT, SPIN_TEXT_SIZE, WHITE, CC_DATUM);
                dPrint(calculateMode?testclass[(i+(j>=SPIN_TEXT_SIZE*LETTER_HEIGHT?1:0))%(maxValue+1)]:String(minValue+(i+(j>=SPIN_TEXT_SIZE*LETTER_HEIGHT?1:0))%(maxValue-minValue+1)), DISPLAY_LENGTH/2, DISPLAY_HEIGHT/2 + j%(SPIN_TEXT_SIZE*LETTER_HEIGHT) + 0.5*SPIN_TEXT_SIZE*LETTER_HEIGHT, SPIN_TEXT_SIZE, WHITE, CC_DATUM);
                display.fillRect(SIDE_MARGIN, DISPLAY_HEIGHT/2 - 2*SPIN_TEXT_SIZE*LETTER_HEIGHT, DISPLAY_LENGTH - 2*SIDE_MARGIN, SPIN_TEXT_SIZE*LETTER_HEIGHT, BLACK);
                display.fillRect(SIDE_MARGIN, DISPLAY_HEIGHT/2 + SPIN_TEXT_SIZE*LETTER_HEIGHT, DISPLAY_LENGTH - 2*SIDE_MARGIN, SPIN_TEXT_SIZE*LETTER_HEIGHT, BLACK);  
                delayMicroseconds(pow(max(i-randomValue-maxValue+7,0), 7));
            }
        }
       // temparray.
    }
}

void RandomStudentGui::drawRandomNumber()
{
    int number = random(1, maxRandomValue + 1);
    dPrint(lastRandomValue, 137, DISPLAY_HEIGHT * 3 / 5 + 16, 6, BLACK, 2);
    lastRandomValue = number;
    dPrint(number, 137, DISPLAY_HEIGHT * 3 / 5 + 16, 6, LIME, 2);
}

void RandomStudentGui::drawMaxRandomValue()
{
    dPrint(lastMaxRandomValue, DISPLAY_LENGTH / 2 + 20, DISPLAY_HEIGHT * 2 / 5, 7, BLACK, 3);
    dPrint(maxRandomValue, DISPLAY_LENGTH / 2 + 20, DISPLAY_HEIGHT * 2 / 5, 7, WHITE, 3);
}

void RandomStudentGui::changeMax(int difference)
{
    lastMaxRandomValue = maxRandomValue;
    maxRandomValue += difference;
    if ((maxRandomValue - difference) <= 0)
    {
        maxRandomValue = 0;
    }
    else if ((maxRandomValue - difference) > 100)
    {
        maxRandomValue -= 100;
    }
    drawMaxRandomValue();
}

void RandomStudentGui::handleTouch(TSPoint p) {
    if(millis()-lastTouch < 200)
        return;
    if(!p.isTouching()) {
        lastTouching = false;
        return;
    }
    lastTouch = millis();
    p.calibrate();
    if(gui.equals(RANDOM_STUDENT_GUI)) { 
        if (p.yc > TOP_MARGIN)
            if (p.xc > DISPLAY_LENGTH - SIDE_MARGIN - TOUCH_FIELD_INCREASE) {
                temparray = testclass;
                if (p.yc < TOP_MARGIN + (DISPLAY_HEIGHT - TOP_MARGIN - LINE_MARGIN) / 2) {
                    calculateMode = true;
                    drawGui();
                } else {
                    calculateMode = false;
                    drawGui();
                }
            } else if (p.xc < SIDE_MARGIN + TOUCH_FIELD_INCREASE) {  
                temparray = testclass;
                dPrint((minRandomValue < 10 ? "0" : "") + String(minRandomValue) + (maxRandomValue < 10 ? "-0" : "-") + String(maxRandomValue), DISPLAY_LENGTH / 2 + 3, TOP_MARGIN + LINE_MARGIN + (DISPLAY_HEIGHT - TOP_MARGIN - LINE_MARGIN) / 2 - 6, 6, BLACK, CC_DATUM);
                if (p.yc < TOP_MARGIN + (DISPLAY_HEIGHT - TOP_MARGIN - LINE_MARGIN) / 2) {
                    if (minValueSelect) {
                        if (minRandomValue < maxRandomValue)
                            minRandomValue+=LONG_PRESS&&minRandomValue+10<=maxRandomValue?10:1;
                        else if (maxRandomValue < 99) {
                            minRandomValue+=LONG_PRESS&&maxRandomValue+10<=99?10:1;
                            maxRandomValue+=LONG_PRESS&&maxRandomValue+10<=99?10:1;
                        }
                    } else if (maxRandomValue < 99)
                        maxRandomValue+=LONG_PRESS&&maxRandomValue+10<=99?10:1;
                    drawGui();
                } else {
                    if (minValueSelect) {
                        if (minRandomValue > 0)
                            minRandomValue-=LONG_PRESS&&minRandomValue>10?10:1;
                    } else if (maxRandomValue > minRandomValue)
                        maxRandomValue-=LONG_PRESS&&maxRandomValue-10>=minRandomValue?10:1;
                    else if (minRandomValue > 0) {
                        minRandomValue-=LONG_PRESS&&minRandomValue>10?10:1;
                        maxRandomValue-=LONG_PRESS&&minRandomValue>10?10:1;
                    }
                    drawGui();
                }
                lastTouching = true;
            } else if (p.yc > 180) {
                drawGui(true);
            } else {
                minValueSelect = !minValueSelect;
                drawGui();
            }
    }
}

