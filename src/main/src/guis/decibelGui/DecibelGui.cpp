
#include "DecibelGui.h"
#define BAR_LENGTH DISPLAY_LENGTH - 2*MARGIN_SIDE

#define STRIPE_HEIGT 10
#define STRIPE_X 40
#define STRIPE_Y 140
#define TOTAL_STRIPES 17

#define BAR_COLOR RED
#define TITLE_COLOR LIGHTGREY

#define AVERAGE_SEGMENTS 1000
#define AVERAGE_TIME 10

long lowestAverage = 0;
long highestAverage = 0;
long decibelAverage = 0;

void DecibelGui::setup() {

}

void DecibelGui::loop() {
    Serial.println("loop");
} 

void DecibelGui::initGui() {
    if(gui.equals(DECIBEL_GUI)) {
        display.fillScreen(BLACK);
        Display::drawTopBar("L" + ae + "rm");
        dPrint("Lautst" + ae + "rke", DISPLAY_LENGTH/2, DISPLAY_HEIGHT/2, 4, TITLE_COLOR, 7);
        drawBar(0, RED);
        /* display.fillRect(MARGIN_SIDE-LINE_THICKNESS, BAR_HEIGHT-LINE_THICKNESS, DISPLAY_LENGTH - 2*MARGIN_SIDE + 2*LINE_THICKNESS, THICKNESS + 2*LINE_THICKNESS, OUTLINE_COLOR);
        display.fillRect(MARGIN_SIDE, BAR_HEIGHT, DISPLAY_LENGTH - 2*MARGIN_SIDE, THICKNESS, BLACK);  */    
        delay(1000);
        for(int x = 0; x <= 100; x += 5) {
            drawBar(x, RED);
            delay(200);
        }
        for(int x = 100; x >= 0; x -= 5) {
            drawBar(x, RED);
            delay(200);
        }
        
    }
}



void DecibelGui::drawBar(int value, int color) {
    /* int pixel = map(value, 1, 100, 1, BAR_LENGTH);
    if((pixel <= BAR_LENGTH) && (pixel > 0)) {
        display.fillRect(MARGIN_SIDE, BAR_HEIGHT, pixel, THICKNESS, BAR_COLOR);
    } else if(pixel > BAR_LENGTH) {
        display.fillRect(MARGIN_SIDE, BAR_HEIGHT, BAR_LENGTH, THICKNESS, BAR_COLOR);
    } */

    int unit = map(value, 1, 100, 1, TOTAL_STRIPES);
    if(unit < 0)
        unit = 0;
    if(unit > TOTAL_STRIPES)
        unit = TOTAL_STRIPES;
    Serial.println(value);
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
        Serial.println(x);
    }
}



