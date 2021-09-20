
#include "DecibelGui.h"
#define BAR_LENGTH DISPLAY_LENGTH - 2*MARGIN_SIDE

#define STRIPE_HEIGT 10
#define STRIPE_X 40
#define STRIPE_Y 140
#define TOTAL_STRIPES 17

#define BAR_COLOR RED
#define TITLE_COLOR LIGHTGREY

#define AVERAGE_SEGMENTS 10
#define AVERAGE_TIME 100
#define TEMP_AVERAGE_SEGMENTS 50
#define CORRECTING_THRESHOLD_PERCENT 0.2


// INFO zum Algo
// 
// Prinzipiell werden die absoluten Werte des Sensors auf ein flexibles Spektrum gemappt.
// Das Spektrum passt sich automatisch an, und dehnt sich individuell auf jeden Sensor an.
// Es wird ein Durchschnitt von Werten über ein bestimmten Zeit ermittelt, liegt der Durchschnitt
// über oder unter dem Spektrum passen sich der 'lowestAverage' und 'highestAverage' an.
//

long lowestAverage = 0;
long highestAverage = 0;
long decibelAverage = 0;
long decibelAverageCounter = 0;
long timeStamp = 0;

void DecibelGui::setup() {

    //auto calibrate the mapping data
    long setupAverage = 0;
    for (int x = 0; x <= TEMP_AVERAGE_SEGMENTS; x++) {
        setupAverage += 50; // !!!
    }
    setupAverage = setupAverage / TEMP_AVERAGE_SEGMENTS;
    lowestAverage = setupAverage;
    highestAverage = setupAverage + 1;

}

void DecibelGui::loop() {

    if(millis() - timeStamp > AVERAGE_TIME) {
        timeStamp = millis();
        decibelAverage += random(0,100); // !!!
        decibelAverageCounter++;
        
        //Wenn die Objekte für den Average collected sind...
        if(decibelAverageCounter >= AVERAGE_SEGMENTS) {
            decibelAverageCounter = 0;           
            decibelAverage = decibelAverage / AVERAGE_SEGMENTS;
            int differenceHighestLowest = highestAverage - lowestAverage;

            if (decibelAverage < (lowestAverage - CORRECTING_THRESHOLD_PERCENT*differenceHighestLowest)) {
                lowestAverage = decibelAverage;
                Serial.print(": changed lowestAverage");
                Serial.println(lowestAverage);
            }
            if (decibelAverage > (highestAverage + CORRECTING_THRESHOLD_PERCENT*differenceHighestLowest)) {
                highestAverage = decibelAverage;
                Serial.print(": changed highestAverage ");
                Serial.println(highestAverage);
            }
            Serial.println(decibelAverage);
            drawBar(decibelAverage, BAR_COLOR);
            // Serial.println(map(decibelAverage, lowestAverage, highestAverage, 1, 100));
            decibelAverage = 0;
        }
    }
} 

void DecibelGui::initGui() {
    if(gui.equals(DECIBEL_GUI)) {
        display.fillScreen(BLACK);
        Display::drawTopBar("L" + ae + "rm");
        dPrint("Lautst" + ae + "rke", DISPLAY_LENGTH/2, DISPLAY_HEIGHT/2, 4, TITLE_COLOR, 7);
        drawBar(0, RED);
    }
}



void DecibelGui::drawBar(int value, int color) {
    int unit = map(value, 1, 100, 1, TOTAL_STRIPES);
    if(unit < 0)
        unit = 0;
    if(unit > TOTAL_STRIPES)
        unit = TOTAL_STRIPES;

    // display.fillRect(0, DISPLAY_HEIGHT/2, DISPLAY_LENGTH, DISPLAY_HEIGHT/2, BLACK);
    drawStripes(unit, color);
}

void DecibelGui::drawStripes(int value, int color) {
    int stripe_height = STRIPE_HEIGT;
    int stripe_x = STRIPE_X;
    for(int x = 1; x <= TOTAL_STRIPES; x++) {
        if(x%3 == 0) {
            stripe_height += STRIPE_HEIGT;
        }
        int randomColor = RED;
        switch(random(1, 6)) {
            case 1: randomColor = 0xF800; break;
            case 2: randomColor = 0xF000; break;
            case 3: randomColor = 0xF8E3; break;
            case 4: randomColor = 0xC020; break;
            case 5: randomColor = 0xFA28; break;
            case 6: randomColor = 0xD1A6; break;
        }
        if(x > value) {
            display.fillRect(stripe_x, STRIPE_Y, 10, stripe_height, GREY);
        } else {
            display.fillRect(stripe_x, STRIPE_Y, 10, stripe_height, randomColor);
        }      
        stripe_x += 14;
    }
}



