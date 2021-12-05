
#include "CalibrateGui.h"

using namespace std;

int _maxPPM = 1500;
int _lastMaxPPM = 0;
long lastCali = 0;
String lastCaliString = "";


long _loopTimestamp = 0;
vector<int> valueArr;    
int lastAllTimeMinValue;
int lastAllTimeMaxValue;

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

    if((millis() - 5000) > _loopTimestamp) {
        _loopTimestamp = millis();
        drawLastCali(160, DISPLAY_HEIGHT*5/6, 3, WHITE, CC_DATUM);
        
        valueArr.insert(valueArr.begin(), Meassure::getRawAirCondition());
        if(valueArr.size() == 60) valueArr.pop_back();

        for(int i : valueArr) {
            Serial.print(i);
        }
        Serial.println();

        int _min = *min_element(valueArr.begin(), valueArr.end());
        int _max = *max_element(valueArr.begin(), valueArr.end());
        drawMinAndMaxValues(_min, _max, 160, DISPLAY_HEIGHT/2, 3, WHITE, CC_DATUM);
    }
}

void CalibrateGui::drawMinAndMaxValues(int min, int max, int x, int y, int size, int color, int datum) {
    dPrint(lastAllTimeMinValue, x, y, size, BLACK, datum);
    dPrint(lastAllTimeMaxValue, x, y + 8*size + 4, size, BLACK, datum);
    lastAllTimeMinValue = min;
    lastAllTimeMaxValue = max;

    dPrint(min, x, y, size, GREEN, datum);
    dPrint(max, x, y + 8*size + 4, size, RED, datum);
    Serial.println(min + "-" + max);
}

void CalibrateGui::handleTouch(TSPoint p) {
    if(!gui.equals(CALIBRATE_GUI)) return;
    if(p.isTouching(0, DISPLAY_LENGTH/2, 40, DISPLAY_HEIGHT*5/8)) {
        _maxPPM -= 25;
    }
    if(p.isTouching(DISPLAY_LENGTH/2, DISPLAY_LENGTH, 40, DISPLAY_HEIGHT*5/8)) {
        _maxPPM += 25;
    }
    if(p.isTouching(0, DISPLAY_LENGTH/2, DISPLAY_HEIGHT*3/4, DISPLAY_HEIGHT)) {
        Meassure::calibrateMin();
        lastCali = millis();
    }
    if(p.isTouching(DISPLAY_LENGTH/2, DISPLAY_LENGTH, DISPLAY_HEIGHT*3/4, DISPLAY_HEIGHT)) {
        Meassure::calibrateMax(_maxPPM);
    }
}
    
void CalibrateGui::drawMaxPPM(int x, int y, int size, int color, int datum) {
    if(_lastMaxPPM == _maxPPM) return;

    dPrint(_lastMaxPPM, x, y, size, BLACK, datum);
    dPrint(_maxPPM, x, y, size, color, datum);
    
    _lastMaxPPM = _maxPPM;
}


void CalibrateGui::autoCalibrate() {
    if(lastCali+10000 > millis()) return;
    if((Meassure::getRawAirCondition() < 395) && (Meassure::isConnected()) && (Meassure::getRawAirCondition() > 20)) {
        Serial.println(Meassure::getRawAirCondition());
        Meassure::forcedMinCalibration();
        lastCali = millis();
        dPrint("(MIN)", DISPLAY_LENGTH*1/5, DISPLAY_HEIGHT*7/8, 3, RED, CC_DATUM);
        delay(6000); 
        dPrint("(MIN)", DISPLAY_LENGTH*1/5, DISPLAY_HEIGHT*7/8, 3, GREY, CC_DATUM); 
    }
}

void CalibrateGui::drawLastCali(int x, int y, int size, int color, int datum) {
    int _output = (millis() - lastCali) / 1000 / 60;
    dPrint(lastCaliString, x, y, size, BLACK, datum);
    dPrint(_output, x, y, size, color, datum);
    lastCaliString = _output;

}

