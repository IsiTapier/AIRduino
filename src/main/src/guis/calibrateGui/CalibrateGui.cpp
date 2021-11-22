
#include "CalibrateGui.h"

int maxPPM = 1500;
int lastMaxPPM = 0;
long lastCali = 0;

void CalibrateGui::initGui() {
    if(gui.equals(CALIBRATE_GUI)) {
        display.fillScreen(BLACK);
        Display::drawTopBar("Calibrate");
        dPrint("Cali", DISPLAY_LENGTH*1/5, DISPLAY_HEIGHT*3/4, 3, LIME, CC_DATUM);
        dPrint("(MIN)", DISPLAY_LENGTH*1/5, DISPLAY_HEIGHT*7/8, 3, GREY, CC_DATUM);

        dPrint("Cali", DISPLAY_LENGTH*4/5, DISPLAY_HEIGHT*3/4, 3, ORANGE, CC_DATUM);
        dPrint("(MAX)", DISPLAY_LENGTH*4/5, DISPLAY_HEIGHT*7/8, 3, GREY, CC_DATUM);

        dPrint("raw", 160, DISPLAY_HEIGHT/4, 3, WHITE, ML_DATUM);
    }
}

void CalibrateGui::loop() {
    if(!gui.equals(CALIBRATE_GUI)) return;
    DisplayVX::drawRawPPMStraight(20, DISPLAY_HEIGHT/4, 4, WHITE, ML_DATUM);
    DisplayVX::drawPPMStraight(20, DISPLAY_HEIGHT/2, 4, WHITE, ML_DATUM);
    drawMaxPPM(DISPLAY_LENGTH*4/5, DISPLAY_HEIGHT/2, 4, COLOR_STATUS_RISK, CC_DATUM);
    autoCalibrate();
}

void CalibrateGui::handleTouch(TSPoint p) {
    if(!gui.equals(CALIBRATE_GUI)) return;
    if(p.isTouching(0, DISPLAY_LENGTH/2, 40, DISPLAY_HEIGHT*5/8)) {
        maxPPM -= 50;
    }
    if(p.isTouching(DISPLAY_LENGTH/2, DISPLAY_LENGTH, 40, DISPLAY_HEIGHT*5/8)) {
        maxPPM += 50;
    }
    if(p.isTouching(0, DISPLAY_LENGTH/2, DISPLAY_HEIGHT*3/4, DISPLAY_HEIGHT)) {
        Meassure::calibrateMin();
    }
    if(p.isTouching(DISPLAY_LENGTH/2, DISPLAY_LENGTH, DISPLAY_HEIGHT*3/4, DISPLAY_HEIGHT)) {
        Meassure::calibrateMax(maxPPM);
    }
}
    
void CalibrateGui::drawMaxPPM(int x, int y, int size, int color, int datum) {
    if(lastMaxPPM == maxPPM) return;

    dPrint(lastMaxPPM, x, y, size, BLACK, datum);
    dPrint(maxPPM, x, y, size, color, datum);
    lastMaxPPM = maxPPM;
}

void CalibrateGui::autoCalibrate() {
    if(lastCali+3000 > millis()) return;
    if((Meassure::getRawAirCondition() < 395) && (Meassure::isConnected())) {
        Serial.println(Meassure::getRawAirCondition());
        Meassure::forcedMinCalibration();
        lastCali = millis();
        dPrint("cali.", DISPLAY_LENGTH/2, DISPLAY_HEIGHT/2, 2, RED, CC_DATUM);
        delay(6000); 
        dPrint("cali.", DISPLAY_LENGTH/2, DISPLAY_HEIGHT/2, 2, BLACK, CC_DATUM);  
    }
}

