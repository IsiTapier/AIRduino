
#include "DecibelGui.h"

#define BAR_LENGTH DISPLAY_LENGTH - 2*MARGIN_SIDE
#define STRIPE_HEIGT 10
#define TRAFFIC_LIGHT_X DISPLAY_LENGTH/2
#define TRAFFIC_LIGHT_Y 140
#define TOTAL_STRIPES 17

#define BAR_COLOR RED
#define TITLE_COLOR LIGHTGREY
#define MARGIN 20

#define AVERAGE_SEGMENTS 10
#define AVERAGE_TIME 1000
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
int trafficLightColor = GREY;

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

        drawTrafficLight(random(1, 100), 1, 100, 160, 135, 35);

        /* decibelAverage += random(0,100); // !!!
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
        } */
    }
} 

void DecibelGui::initGui() {
    if(gui.equals(DECIBEL_GUI)) {
        display.fillScreen(BLACK);
        Display::drawTopBar("L" + ae + "rm");
        dPrint("Sensibilit" + ae + "t", 160, 235, 2, GREY, 7);
        dPrint("L" + ae + "rmampel", DISPLAY_LENGTH/2, 90, 4, TITLE_COLOR, 7);
        //drawBar(0, RED);
        drawSlider(50, MARGIN, 195, DISPLAY_LENGTH-MARGIN*2, 5);
    }
}



void DecibelGui::drawBar(int value, int color) {
    int unit = map(value, 1, 100, 1, TOTAL_STRIPES);
    constrain(unit, 0, TOTAL_STRIPES);

    // display.fillRect(0, DISPLAY_HEIGHT/2, DISPLAY_LENGTH, DISPLAY_HEIGHT/2, BLACK);
    drawStripes(unit, color);
}

void DecibelGui::drawStripes(int value, int color) {
    int stripe_height = STRIPE_HEIGT;
    int stripe_x = TRAFFIC_LIGHT_X;
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
            display.fillRect(stripe_x, TRAFFIC_LIGHT_Y, 10, stripe_height, GREY);
        } else {
            display.fillRect(stripe_x, TRAFFIC_LIGHT_Y, 10, stripe_height, randomColor);
        }      
        stripe_x += 14;
    }
}

void DecibelGui::drawTrafficLight(int value, int startA, int startB, int x, int y, int size) {
    int lightDigit = map(value, startA, startB, 1, 3);
    int circleDistance = size*2.5;
    for(int c = -1; c <= 1; c++) {
        display.fillCircle(x - c*circleDistance, y, size, GREY);
    }

    if(lightDigit <= 1) {
        trafficLightColor = COLOR_STATUS_NORMAL;
        display.fillCircle(x-circleDistance, y, size, COLOR_STATUS_NORMAL);
    }
    if(lightDigit == 2) {
        trafficLightColor = COLOR_STATUS_RISK;
        display.fillCircle(x, y, size, COLOR_STATUS_RISK);
    }
    if(lightDigit >= 3) {
        trafficLightColor = COLOR_STATUS_ALARM;
        display.fillCircle(x+circleDistance, y, size, COLOR_STATUS_ALARM);
    }
    if(gui.equals(DECIBEL_GUI)) dPrint("Sensibilit" + ae + "t", 160, 235, 2, trafficLightColor, 7);    
}

void DecibelGui::registerTouch(TSPoint ts) {
    if(!gui.equals(DECIBEL_GUI)) return;
    
    if(ts.isTouching(MARGIN, DISPLAY_LENGTH-2*MARGIN, 120, 240)) {
        int x = map(ts.x, EEPROM.readShort(XMIN), EEPROM.readShort(XMAX), MARGIN, DISPLAY_LENGTH-MARGIN*2);
        drawSlider(x, MARGIN, 195, DISPLAY_LENGTH-MARGIN*2, 5);
    }
    
}

void DecibelGui::drawSlider(int sliderX, int x, int y, int length, int height) {

    display.fillRect(0, y-height*4, DISPLAY_LENGTH, height*8, BLACK);
    display.fillRect(x, y, length, height, LIGHTGREY);
    display.fillRect(sliderX, y, height*2, height*3, LIGHTGREY);
    display.fillRect(sliderX, y-height*2, height*2, height*2, LIGHTGREY);

}



