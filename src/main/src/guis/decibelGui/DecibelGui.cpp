
#include "DecibelGui.h"


//design
#define BAR_LENGTH DISPLAY_LENGTH - 2*MARGIN_SIDE
#define STRIPE_HEIGT 10
#define TRAFFIC_LIGHT_X DISPLAY_LENGTH/2
#define TRAFFIC_LIGHT_Y 140
#define TOTAL_STRIPES 17

#define BAR_COLOR RED
#define TITLE_COLOR LIGHTGREY
#define MARGIN 30

//Algo
#define AVERAGE_ELEMENTS 100
#define TOTAL_LOOP_LENGTH 2000
#define LOOP_LENGTH TOTAL_LOOP_LENGTH / AVERAGE_ELEMENTS
#define AMOUNT_OF_PEEPS 2


int trafficLightColor = GREY;

int initialValue = 0; //Totpunkt der Amplitude
long timeStamp = 0;
int loopCounter = AVERAGE_ELEMENTS;

int measuredValue = 0;
double _sliderFactor = 1;
int sliderX = MARGIN;
int lastTrafficDigit = -10;

int DecibelGui::trafficLightStage = 1;
boolean DecibelGui::isActive = true;
int DecibelGui::loopAverage = 0;


void DecibelGui::setup() {
//calculate initialValue
    for (int c = 0; c <= 2000; c++) {
        measuredValue = analogRead(MICROPHONE_MODULE);
        initialValue += measuredValue;
        delay(3);
    }
    initialValue = initialValue / 2000;

    if(initialValue >= 50) {
        Serial.print("Lärmampel: initialValue:");
        Serial.println(initialValue);
        Serial.print("Lärmampel: LOOP_LENGTH:");
        Serial.println(LOOP_LENGTH);
    } else {
        Serial.println("Lärmampel ist nicht aktiv");
        DecibelGui::isActive = false;
    }
}

void DecibelGui::loop() {
    if(!isActive) return;

    if ((millis() - timeStamp) > LOOP_LENGTH) {
        timeStamp = millis();
        measuredValue = analogRead(MICROPHONE_MODULE);

        if(measuredValue > initialValue) {
            loopAverage += measuredValue;
            //Serial.println(measuredValue);
        } else {
            //Serial.println(2*initialValue - measuredValue);
            loopAverage += 2*initialValue - measuredValue;
        }

        if(loopCounter <= 0) {
            loopAverage = loopAverage / AVERAGE_ELEMENTS;
            loopCounter = AVERAGE_ELEMENTS;
            Meassure::decibelValue = loopAverage;
            
            Serial.println("----");
            Serial.println(DisplayVX::recentPeepStatus);
            if(!DisplayVX::recentPeepStatus) { //verhindert kettenreaktion zwischen piepser des CO2s und dem Mikro
                int oldTrafficLightStage = trafficLightStage;
                trafficLightStage = map(loopAverage, initialValue, 2.25*initialValue - _sliderFactor, 0, 4);

                if(gui.equals(DECIBEL_GUI)) {
                    drawTrafficLight(trafficLightStage, 160, 135, 35);
                    display.fillRect(0, 200, 70, 40, BLACK);
                    dPrint(loopAverage, 10, DISPLAY_HEIGHT-10, 2, WHITE, 6);
                }
                
                Serial.print(initialValue);
                Serial.print(" - ");
                Serial.print(loopAverage);
                Serial.print(" - ");
                Serial.println(2.25*initialValue - _sliderFactor);
                
                if(trafficLightStage >= 3) {
                    if(oldTrafficLightStage != trafficLightStage) {
                        if(gui.equals(DECIBEL_GUI) || gui.equals(OVERVIEW_GUI)) {
                            TimerGui::peepCount += AMOUNT_OF_PEEPS * 2;

                        }
                        
                    }
                }
            }
            if(TimerGui::peepCount > 0) {
                DisplayVX::recentPeepStatus = true;
            } else {
                DisplayVX::recentPeepStatus = false;
            }
            
        }
        loopCounter--;
    }
} 

void DecibelGui::initGui() {
    if(gui.equals(DECIBEL_GUI)) {
        display.fillScreen(BLACK);
        Display::drawTopBar("L" + ae + "rm");
        dPrint("Sensibilit" + ae + "t", 160, 235, 2, GREY, 7);
        dPrint("L" + ae + "rmampel", DISPLAY_LENGTH/2, 90, 4, TITLE_COLOR, 7);
        //drawBar(0, RED);
        drawSlider(sliderX, MARGIN, 195, DISPLAY_LENGTH-MARGIN*2, 5);
        drawTrafficLight(-999, 160, 135, 35);
        drawSliderFactor(round(_sliderFactor/10));

        if(!DecibelGui::isActive) dPrint("Deaktiviert", DISPLAY_LENGTH/2, 50, 2, COLOR_STATUS_ALARM, 7);
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

void DecibelGui::drawTrafficLight(int lightDigit, int x, int y, int size) {
    if(lastTrafficDigit == lightDigit) return;

    lastTrafficDigit = lightDigit;

    int circleDistance = size*2.5;
    for(int c = -1; c <= 1; c++) {
        display.fillCircle(x - c*circleDistance, y, size, GREY);
    }

    if((lightDigit <= 1) && (lightDigit != -999)) {
        trafficLightColor = COLOR_STATUS_NORMAL;
        display.fillCircle(x-circleDistance, y, size, COLOR_STATUS_NORMAL);
    } else if(lightDigit == 2) {
        trafficLightColor = COLOR_STATUS_RISK;
        display.fillCircle(x, y, size, COLOR_STATUS_RISK);
    } else if(lightDigit >= 3) {
        trafficLightColor = COLOR_STATUS_ALARM;
        display.fillCircle(x+circleDistance, y, size, COLOR_STATUS_ALARM);
    }
    if(gui.equals(DECIBEL_GUI)) dPrint("Sensibilit" + ae + "t", 160, 235, 2, trafficLightColor, 7);    
}

void DecibelGui::registerTouch(TSPoint ts) {
    if(!gui.equals(DECIBEL_GUI)) return;
    
    if(ts.isTouching(0, DISPLAY_LENGTH, 80, 240)) {
        sliderX = map(ts.x, EEPROM.readShort(XMIN), EEPROM.readShort(XMAX), MARGIN, DISPLAY_LENGTH-MARGIN*2);
        drawSlider(sliderX, MARGIN, 195, DISPLAY_LENGTH-MARGIN*2, 5);
        _sliderFactor = map(sliderX, MARGIN, DISPLAY_LENGTH-MARGIN, 0, initialValue);
        constrain(_sliderFactor, 0, initialValue/2);

        display.fillRect(240, 210, 80, 40, BLACK);
        drawSliderFactor(round(_sliderFactor/10));

    }
}

void DecibelGui::drawSlider(int sliderX, int x, int y, int length, int height) {

    display.fillRect(0, y-height*4, DISPLAY_LENGTH, height*8, BLACK);
    display.fillRoundRect(x, y, length, height, 3, LIGHTGREY);
    display.fillRect(sliderX, y, height*2, height*3, LIGHTGREY);
    display.fillRect(sliderX, y-height*2, height*2, height*2, LIGHTGREY);

}

void DecibelGui::drawSliderFactor(int value) {
    dPrint(round(_sliderFactor/10), DISPLAY_LENGTH-MARGIN, 235, 2, LIGHTGREY, 8);
    dPrint("x", DISPLAY_LENGTH-MARGIN + 3, 235, 2, LIGHTGREY, 6);
    Serial.print("Slider Factor: ");
    Serial.println(_sliderFactor);  
}

