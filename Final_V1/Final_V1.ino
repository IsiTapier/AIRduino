



#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>



//                                      ,,               ,,
//      db      `7MMF'`7MM"""Mq.      `7MM               db
//     ;MM:       MM    MM   `MM.       MM
//    ,V^MM.      MM    MM   ,M9   ,M""bMM `7MM  `7MM  `7MM  `7MMpMMMb.  ,pW"Wq.
//   ,M  `MM      MM    MMmmdM9  ,AP    MM   MM    MM    MM    MM    MM 6W'   `Wb
//   AbmmmqMA     MM    MM  YM.  8MI    MM   MM    MM    MM    MM    MM 8M     M8
//  A'     VML    MM    MM   `Mb.`Mb    MM   MM    MM    MM    MM    MM YA.   ,A9
//.AMA.   .AMMA..JMML..JMML. .JMM.`Wbmd"MML. `Mbod"YML..JMML..JMML  JMML.`Ybmd9'
//

//   _____             __ _
//  / ____|           / _(_)
// | |     ___  _ __ | |_ _  __ _
// | |    / _ \| '_ \|  _| |/ _` |
// | |___| (_) | | | | | | | (_| |
// \_____\___/|_| |_|_| |_|\__, |
//                           __/ |
//                          |___/
//
//variabel defines:
#define ROTATION 45
#define DISPLAY_BRIGHTNESS 0.7
#define DEBUG false
#define FACTOR 4.631578947368421

#define LOADING_SCREEN_TIME 2
#define STAGE_TIME 1*1000
#define VENTILATING_TIMEOUT 5*1000

#define AVERAGING_MEASUREMENTS 100
#define AVERAGING_GRADIENT 5
#define AVERAGING_GRAPH 10
#define ALPHA_MEASUREMENTS 0.7
#define ALPHA_LOWEST 0.5
#define ALPHA_GRAPH 0.7

#define OSV_SENSOR 80
#define OSV_PPM 400
#define MIN_HIGHT DISPLAY_WIDTH - map(OSV_PPM, 0, MAX_DISPLAYED_PPM, 0, DISPLAY_WIDTH)
#define CRITICAL_HIGHT DISPLAY_WIDTH - map(MAX_LIGHT, 0, MAX_DISPLAYED_PPM, 0, DISPLAY_WIDTH)

#define MAX_SENSOR 1023
#define MAX_PPM 5000
#define MAX_INCREASE 1.5
#define MAX_DECREASE 0.96  //Schwelle ab der die Ventilation getriggert wird
#define MAX_INCREASE_LOWEST 50
#define MAX_LIGHT 1000
#define MAX_BLINK 1100
#define MAX_PIEP 1200
#define MAX_DISPLAYED_PPM 1280
#define MAX_DISPLAYED_SENSOR 270

//pins:
#define TFT_CS     10
#define TFT_RST    9
#define TFT_DC     8
#define TFT_LED    3
#define LED_RED    5
#define LED_GREEN  6
#define LED_BLUE   7
#define GAS_SENSOR A0
#define PIEZO      4

//Data:
#define DISPLAY_LENGTH 159
#define DISPLAY_WIDTH 127

//Design
#define DATABOX_TOP_HIGHT 100
#define GRAPH_COLOR WHITE//MAGENTA
#define GRAPH_BACKGROUND_COLOR BLACK
#define BAR_BACKGROUND_COLOR 0x2104 //Fast schwarz
#define BAR_STRIPE_THICKNESS 3
#define TIME_COLOR_CRITICAL RED
#define TIME_COLOR_NORMAL WHITE
#define LOADING_SCREEN_DOTS_COLOR WHITE


// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define ORANGE 0xFC00
#define WHITE 0xFFFF
#define GREY 0x8C51
#define LIME 0x87F4
#define LIGHT_BLUE 0x76BF
#define TURKISE 0x3FFA

#define PPM_COLOR_N GREEN //Normal
#define PPM_COLOR_R 0xFE60 //Risk
#define PPM_COLOR_A 0xFA27 //Alarm


enum State {VENTILATING = -1, GOOD = 0, MEDIUM = 1, RISKY = 2, BAD = 3, BLINK = 4, PIEP = 5};

State states[8] = {State::VENTILATING, State::GOOD, State::MEDIUM, State::RISKY, State::BAD, State::BLINK, State::PIEP};

int getColor(State state) {
  switch ((int)state) {
    case -1:  return (CYAN);
      break;
    case 0: return (PPM_COLOR_N);
      break;
    case 1: return (PPM_COLOR_R);
      break;
    case 2: return (ORANGE);
      break;
    case 3: return (PPM_COLOR_A);
      break;
    default: return (PPM_COLOR_A);
      break;
  }
}

State getState(int value) {
  for (int i = 0; i < 8; i++) {
    if ((int) states[i] == value)
      return (states[i]);
  }
}



Adafruit_ST7735 display = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// __      __        _       _     _
// \ \    / /       (_)     | |   | |
//  \ \  / /_ _ _ __ _  __ _| |__ | | ___ _ __
//   \ \/ / _` | '__| |/ _` | '_ \| |/ _ \ '_ \ 
//    \  / (_| | |  | | (_| | |_) | |  __/ | | |
//     \/ \__,_|_|  |_|\__,_|_.__/|_|\___|_| |_|
//

short graphData[DISPLAY_LENGTH];
unsigned long startTime;
unsigned long timer;
String lastTime;
String Time;
short seconds;
short minutes;
int airCondition = 0;
short lastAirCondition = 0;
short airConditionRaw = 0;
short value;
short lowest = 600;
short led;
short red;
short green;
short values[AVERAGING_GRADIENT * 2];
short last;
short now;
short gradient;
short drop;
State state;
short colorState;
short lastAirConditionGraph;
int valuesGraph[AVERAGING_GRAPH];
int counter = 0;
int pixel = 0;
int lastPixel = 0;
int lastState = false;

short previousState;
int ppmSinceVentilation;


//   _____      _
//  / ____|    | |
// | (___   ___| |_ _   _ _ __
//  \___ \ / _ \ __| | | | '_ \ 
//  ____) |  __/ |_| |_| | |_) |
// |_____/ \___|\__|\__,_| .__/
//                       | |
//                       |_|
void setup() {
  Serial.begin(9600);
  initDisplay();
  initSensor();
  loadingScreen(LOADING_SCREEN_TIME);
}


//  _
// | |
// | |     ___   ___  _ __
// | |    / _ \ / _ \| '_ \ 
// | |___| (_) | (_) | |_) |
// |______\___/ \___/| .__/
//                   | |
//                   |_|
void loop() {
  meassureAirCondition();
  mapAirCondition();
  calculateGradient();
  checkVentilating();
  setStatus();
  writeLed();
  draw(map(airCondition, 0, MAX_DISPLAYED_PPM, 0, DISPLAY_WIDTH));
  debugSensor();
}
//   _____
//  / ____|
// | (___   ___ _ __  ___  ___  _ __
//  \___ \ / _ \ '_ \/ __|/ _ \| '__|
//  ____) |  __/ | | \__ \ (_) | |
// |_____/ \___|_| |_|___/\___/|_|
//


void initSensor() {
  //Pins
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(PIEZO, OUTPUT);

  digitalWrite(PIEZO, LOW);

  airConditionRaw = analogRead(GAS_SENSOR);
  lastAirCondition = analogRead(GAS_SENSOR);
  startTime = millis();
}

void debugSensor() {
  if (DEBUG) {
    Serial.println("");
    Serial.println("Sensor");
    Serial.println("");
    debug("Analog", analogRead(GAS_SENSOR));
    debug("Average", airConditionRaw);
    debug("Smoothed", lastAirCondition);
    debug("PPM", airCondition);
    debug("gradient", gradient);
    debug("Lowest", lowest);
    debug("Status", (int) state);
  }
}

void writeLed() {
  //map Values
  led = map(airCondition, lowest, MAX_LIGHT, 0, 255);
  red = led;
  green = 255 - led;

  //turn on
  if (state != VENTILATING)
    rgb(red, green, 0);
}

//  __  __
// |  \/  |
// | \  / | ___  __ _ ___ ___ _   _ _ __ ___
// | |\/| |/ _ \/ _` / __/ __| | | | '__/ _ \
// | |  | |  __/ (_| \__ \__ \ |_| | | |  __/
// |_|  |_|\___|\__,_|___/___/\__,_|_|  \___|



void meassureAirCondition() {
  //Messung
  airCondition = 0;
  for (int i = 0; i < AVERAGING_MEASUREMENTS; i++) {
    value = analogRead(GAS_SENSOR);

    //Fehlmessungen überschreiben
    if (airConditionRaw * MAX_INCREASE < value && i == 0)
      airCondition = airCondition + airConditionRaw;
    else if (airCondition / i * MAX_INCREASE < value && i != 0)
      airCondition = airCondition + airCondition / i;
    else
      airCondition = airCondition + value;

    delay(STAGE_TIME / AVERAGING_MEASUREMENTS);
  }

  airCondition = airCondition / AVERAGING_MEASUREMENTS;
  airConditionRaw = airCondition;

  //Wert smoothen;
  airCondition = ALPHA_MEASUREMENTS * airCondition + (1 - ALPHA_MEASUREMENTS) * lastAirCondition;

  lastAirCondition = airCondition;

}


//  _____        _
// |  __ \      | |
// | |  | | __ _| |_ __ _
// | |  | |/ _` | __/ _` |
// | |__| | (_| | || (_| |
// |_____/ \__,_|\__\__,_|
//
void mapAirCondition() {
  //to PPM
  /*if (airCondition < OSV_SENSOR)
    airCondition = OSV_SENSOR;
    if (airCondition > MAX_DISPLAYED_SENSOR)
    airCondition = MAX_DISPLAYED_SENSOR;*/

  airCondition = map(airCondition, OSV_SENSOR, MAX_DISPLAYED_PPM / FACTOR, OSV_PPM, MAX_DISPLAYED_PPM);
  /*airCondition = airCondition - OSV_SENSOR;
    airCondition = airCondition * FACTOR;
    airCondition = airCondition + OSV_PPM;*/
}



void calculateGradient() {
  // gradient ist die Differenz zwischen altem und neuem Wert
  //store last AirConditions
  for (int i = AVERAGING_GRADIENT * 2 - 1; i > 0; i--) {
    values[i] = values[i - 1];
  }
  values[0] = airCondition;

  //average AirConditions
  now = average(values, 0, AVERAGING_GRADIENT);
  last = average(values, AVERAGING_GRADIENT, AVERAGING_GRADIENT * 2);

  //gradient
  gradient = now / last;
}

void checkVentilating() {
  //start Ventilating
  if (gradient < MAX_DECREASE && state != VENTILATING) { // Wenn die Differenz die Hemmschwelle übersteigt: Wird Ventilating erkannt
    state = VENTILATING;
    ppmSinceVentilation = airCondition;
    rgb(0, 0, 255);
    startTime = millis();
  }

  //stop Ventilating

  if ((gradient > 1 && state == VENTILATING) || (millis() - timer >= VENTILATING_TIMEOUT && state == VENTILATING && timer != 0)) {
    // Wenn der Graph nach oben Steigt oder der Timer abgelaufen ist
    if (airCondition - lowest < MAX_INCREASE_LOWEST) {
      //Wenn
      lowest = ALPHA_LOWEST * airCondition + (1 - ALPHA_LOWEST) * lowest;
    }

    state = 0;
    timer = 0;
    startTime = millis();


  } else if (gradient == 1 && timer == 0 && state == VENTILATING) {
    //Wenn der Graph nicht mehr steigt und der Timer noch nicht gestartet wurde
    //Ziel nach gewisser Zeit kein Abstieg -> Timer für Timeout wird gestartet
    timer = millis();
  }
  last = now;
}

void setStatus() {
  colorState = map(airCondition, lowest, MAX_LIGHT, 0, 3);
  if (state == -1)
    return;
  state = getState(colorState);
  if (airCondition > MAX_PIEP)
    state = PIEP;
  else if (airCondition > MAX_BLINK)
    state = BLINK;

  if (state < -1)
    state = -1;
  if (state > 5)
    state = 5;
}

void checkState() {
  if (lastState) {
    drawBorder(0, 0, DISPLAY_LENGTH, DISPLAY_WIDTH, GRAPH_BACKGROUND_COLOR);
    display.fillRect(0, DATABOX_TOP_HIGHT, DISPLAY_LENGTH, DISPLAY_WIDTH, BAR_BACKGROUND_COLOR);
    createLines();
    digitalWrite(PIEZO, LOW);
    lastState = false;
  } else if (state >= 4) {
    drawBorder(0, 0, DISPLAY_LENGTH, DISPLAY_WIDTH, WHITE);
    if (state == PIEP) {
      //PIEP
      digitalWrite(PIEZO, HIGH);
    }
    lastState = true;
  }
  writeInfo();
}

boolean getData(int data) {
  valuesGraph[counter] = data;
  counter ++;
  if (!(counter < AVERAGING_GRAPH)) {
    pixel = average(valuesGraph, 0, AVERAGING_GRAPH);
    pixel = ALPHA_GRAPH * pixel + (1 - ALPHA_GRAPH) * lastPixel;
    lastPixel = pixel;
    counter = 0;
    return (true);
  } else
    return (false);
}

// Künstliches Auffüllen der Werte, wird später vom Modul übernommen
void fillData(int data) {
  for (short x = DISPLAY_LENGTH; x > 0; x--) {
    graphData[x] = graphData[x - 1];
  }
  graphData[0] = DISPLAY_WIDTH - data;
}


//  _____  _           _
// |  __ \(_)         | |
// | |  | |_ ___ _ __ | | __ _ _   _
// | |  | | / __| '_ \| |/ _` | | | |
// | |__| | \__ \ |_) | | (_| | |_| |
// |_____/|_|___/ .__/|_|\__,_|\__, |
//              | |             __/ |
//              |_|            |___/
//
void initDisplay() {
  pinMode(TFT_CS, OUTPUT);
  pinMode(TFT_RST, OUTPUT);
  pinMode(TFT_DC, OUTPUT);
  pinMode(TFT_LED, OUTPUT);

  analogWrite(TFT_LED, DISPLAY_BRIGHTNESS * 256);

  display.initR(INITR_BLACKTAB);
  display.setTextWrap(false);
  display.setRotation(ROTATION);

  //Auffüllen des Arrays
  for (short x = 0; x < DISPLAY_LENGTH; x++) {
    graphData[x] = DISPLAY_LENGTH;
  }
  graphData[0] = DATABOX_TOP_HIGHT - 1;
}

void draw(int data) {
  checkState();
  if (getData(data)) {
    createLines();
    fillData(pixel);
    drawGraph();
  }
}



void drawBorder(int xStart, int yStart, int xEnd, int yEnd, int color) {
  display.drawLine(xStart, yStart, xEnd, yStart, color);
  display.drawLine(xStart, yEnd, xEnd, yEnd, color);
  display.drawLine(xStart, yStart, xStart, yEnd, color);
  display.drawLine(xEnd, yStart, xEnd, yEnd, color);
}

void drawDisplay() {
  display.fillScreen(GRAPH_BACKGROUND_COLOR);
  display.fillRect(0, DATABOX_TOP_HIGHT, DISPLAY_LENGTH + 1, DISPLAY_WIDTH, BAR_BACKGROUND_COLOR);
  createLines();
}

void createLines() {
  drawLine(DISPLAY_LENGTH + 1, CRITICAL_HIGHT, 5);
  drawLine(DISPLAY_LENGTH + 1, MIN_HIGHT, 5);
}

void drawLine(int x, int y, int z) {
  for (int i = 0; i < x; i = i + z) {
    display.drawPixel(i, y, WHITE);
  }
}

void drawGraph() {
  display.drawLine(DISPLAY_LENGTH, 0, DISPLAY_LENGTH, DATABOX_TOP_HIGHT - 1, GRAPH_BACKGROUND_COLOR);
  for (short x = 0; x <= DISPLAY_LENGTH; x++) {
    byte arrayDigit = x;
    if (graphData[arrayDigit] < DATABOX_TOP_HIGHT)
      display.drawPixel(x, graphData[arrayDigit], GRAPH_COLOR);
    if (x != 0) {
      if (graphData[arrayDigit - 1] < graphData[arrayDigit])
        drawConnections(x, graphData[arrayDigit - 1] + 1, graphData[arrayDigit] + 1);
      else
        drawConnections(x, graphData[arrayDigit], graphData[arrayDigit - 1]);
    }
  }
}

void drawConnections(int x, int startY, int endY) {
  for (byte y = startY; y < endY; y++) {
    if (y < DATABOX_TOP_HIGHT) {
      display.drawPixel(x - 1, y, GRAPH_BACKGROUND_COLOR);
      display.drawPixel(x, y, GRAPH_COLOR);
    }
  }
}

//Loading Screen
void loadingScreen(int t) {
  display.fillScreen(GRAPH_BACKGROUND_COLOR);
  writeLoadingScreenTitle();

  //Draw: Loading Dots
  byte c = 0;
  for (int x = t * 2; x >= 0; x--) {
    c++;
    dPrint(55, 75, 3, GRAPH_BACKGROUND_COLOR, "..."); // Altes Clearen
    switch (c) {
      case 1: dPrint(55, 75, 3, LOADING_SCREEN_DOTS_COLOR, ".");
        break;
      case 2: dPrint(55, 75, 3, LOADING_SCREEN_DOTS_COLOR, "..");
        break;
      case 3: dPrint(55, 75, 3, LOADING_SCREEN_DOTS_COLOR, "...");
        c = 0;
        break;
    }
    delay(500);
  }
  drawDisplay();
}

//Write PPM, Time
void writeInfo() {
  //ppm zeichnen
  if (lastAirConditionGraph != airCondition || previousState != state) {
    //Wenn sich der Wert geändert hat oder state sich geändert hat

    //Schreibt Status an die Decke
    String statusInfo;
    switch (state) {
      case -1: statusInfo = "Lueftet..";
        break;
      case 0: statusInfo = "Optimal";
        break;
      case 1: statusInfo = "Schlecht";
        break;
      case 2: statusInfo = "Riskant";
        break;
      case 3: statusInfo = "Gammlig";
        break;
      case 4: statusInfo = "Gammlig";
        break;
    }
    display.fillRect(30, 0, 110, 15, GRAPH_BACKGROUND_COLOR);
    dPrint(30, 0, 2, WHITE, statusInfo);

    //Verhindert überschreiben von "ppm"
    if (airCondition < 1000)
      dPrint(57, 118, 1.5, getColor(state), "ppm");
    else
      dPrint(57, 118, 1.5, BAR_BACKGROUND_COLOR, "ppm");

    //Clear old Pixels
    dPrint(2, 105, 3, BAR_BACKGROUND_COLOR, lastAirConditionGraph);
    //write new Pixels
    dPrint(2, 105, 3, getColor(state), airCondition);
    //Set new lastAirCondition
    lastAirConditionGraph = airCondition; //Setzt letzten Wert
    //Draw Bar
    display.fillRect(0, DATABOX_TOP_HIGHT, DISPLAY_LENGTH + 1, BAR_STRIPE_THICKNESS, getColor(state));
    previousState = state;
  }
  //Draw Loading Bar
  if (state == VENTILATING) {
    short bar_ventilating_length = map(airCondition, lowest, ppmSinceVentilation, 0, DISPLAY_LENGTH);
    display.fillRect(DISPLAY_LENGTH - bar_ventilating_length, DATABOX_TOP_HIGHT, bar_ventilating_length, BAR_STRIPE_THICKNESS, GREY);
  }


  //calculate time since last ventilating
  seconds = (millis() - startTime) / 1000 % 60;
  minutes = ((millis() - startTime) / 1000 - seconds) / 60;

  //create String
  Time = "";
  if (minutes < 10)
    Time = Time + 0;
  Time = Time + minutes;
  Time = Time + ":";
  if (seconds < 10)
    Time = Time + 0;
  Time = Time + seconds;

  //Clear old Pixels
  dPrint(96, 109, 2, BAR_BACKGROUND_COLOR, lastTime);
  //write new Pixels
  if (minutes >= 20)
    dPrint(96, 109, 2, TIME_COLOR_CRITICAL, Time);
  else
    dPrint(96, 109, 2, TIME_COLOR_NORMAL, Time);
  //Set new lasttime
  lastTime = Time; //Setzt letzten Wert
}


//  ______                _   _
// |  ____|              | | (_)
// | |__ _   _ _ __   ___| |_ _  ___  _ __  ___
// |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
// | |  | |_| | | | | (__| |_| | (_) | | | \__ \
// |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/



void debug(String title, int value) {
  Serial.print(title + ": ");
  Serial.println(value);
}

//Verkürzung: Writing mit Text
void dPrint(int x, int y, float scale, int color, String text) {
  display.setCursor(x, y);
  display.setTextSize(scale);
  display.setTextColor(color);
  display.println(text);
}

//Verkürzung: Writing mit Integern
void dPrint(int x, int y, float scale, int color, int text) {
  display.setCursor(x, y);
  display.setTextSize(scale);
  display.setTextColor(color);
  display.println(text);
}

// Schreibt AIRduino fix aufs Display
void writeLoadingScreenTitle() {
  dPrint(50, 35, 4, LIGHT_BLUE, "A");
  dPrint(70, 35, 4, TURKISE, "I");
  dPrint(90, 35, 4, LIME, "R");
  dPrint(35, 65, 3, GREY, "duino");
}

void rgb(int red, int green, int blue) {
  analogWrite(LED_RED, red);
  analogWrite(LED_GREEN, green);
  analogWrite(LED_BLUE, blue);
}

int average(int averageArray[], int averageStart, int averageEnd) {
  int sum = 0;
  for (int i = averageStart; i < averageEnd; i++) {
    sum = sum + averageArray[i];
  }
  return (sum / (averageEnd - averageStart));
}
