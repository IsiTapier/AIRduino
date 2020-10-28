#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

//variabel defines:
#define ROTATION 45
#define DISPLAY_BRIGHTNESS 0.7
#define DEBUG true

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
#define MAX_DECREASE 30
#define MAX_DROP_INCREASE 50
#define MAX_LIGHT 1000
#define MAX_BLINK 1100
#define MAX_PIEP 1200
#define MAX_DISPLAYED_PPM 1280
#define MAX_DISPLAYED_SENSOR 300
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


Adafruit_ST7735 display = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

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
short pitch;
short drop;
boolean ventilating = false;
short state;
short lastAirConditionGraph;
int valuesGraph[AVERAGING_GRAPH];
int counter = 0;
int pixel = 0;
int lastPixel = 0;
int lastState = false;



void setup() {
  Serial.begin(9600);
  initDisplay();
  initSensor();
  loadingScreen(LOADING_SCREEN_TIME);
}

void loop() {
  meassureAirCondition();
  mapAirCondition();
  calculatePitch();
  checkVentilating();
  setStatus();
  writeLed();
  draw(map(airCondition, 0, MAX_DISPLAYED_PPM, 0, DISPLAY_WIDTH));
  debugSensor();
}

void debug(String title, int value) {
  Serial.print(title + ": ");
  Serial.println(value);
}

int average(int averageArray[], int averageStart, int averageEnd) {
  int back = 0;
  for (int i = averageStart; i < averageEnd; i++) {
    back = back + averageArray[i];
  }
  return (back / (averageEnd - averageStart));
}


//Sensor

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
    debug("Pitch", pitch);
    debug("Lowest", lowest);
    debug("Status", state);
  }
}

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

void mapAirCondition() {
  //to PPM
  if (airCondition < OSV_SENSOR)
    airCondition = OSV_SENSOR;
  if(airCondition > MAX_DISPLAYED_SENSOR)
    airCondition = MAX_DISPLAYED_SENSOR;

  //airCondition = airCondition - OSV_SENSOR;
  airCondition = map(airCondition, OSV_SENSOR, MAX_DISPLAYED_SENSOR, OSV_PPM, MAX_DISPLAYED_PPM);

  /*if (airCondition > MAX_DISPLAYED_PPM)
    airCondition = MAX_DISPLAYED_PPM; */
}

void calculatePitch() {
  //store last AirConditions
  for (int i = AVERAGING_GRADIENT * 2 - 1; i > 0; i--) {
    values[i] = values[i - 1];
  }
  values[0] = airCondition;

  //average AirConditions
  now = average(values, 0, AVERAGING_GRADIENT);
  last = average(values, AVERAGING_GRADIENT, AVERAGING_GRADIENT * 2);

  //Pitch
  pitch = now - last;
}

void checkVentilating() {
  //lowest value
  if (ventilating && airCondition < drop)
    drop = airCondition;

  //start Ventilating
  if (pitch * -1 > MAX_DECREASE && !ventilating) {
    ventilating = true;
    drop = airCondition;
    rgb(0, 0, 255);
    startTime = millis();
  }

  //stop Ventilating
  if (last < now && ventilating || millis() - timer >= VENTILATING_TIMEOUT && ventilating && timer != 0) {
    if (drop - lowest < MAX_DROP_INCREASE) {
      drop = ALPHA_LOWEST * drop + (1 - ALPHA_LOWEST) * lowest;
      lowest = drop;
    }

    ventilating = false;
    timer = 0;
    startTime = millis();
  } else if (now == 0 && timer == 0 && ventilating) {
    timer = millis();
  }
  last = now;
}

void writeLed() {
  //map Values
  led = map(airCondition, lowest, MAX_LIGHT, 0, 255);
  red = led;
  green = 255 - led;

  //turn on
  if (!ventilating)
    rgb(red, green, 0);
}

void rgb(int red, int green, int blue) {
  analogWrite(LED_RED, red);
  analogWrite(LED_GREEN, green);
  analogWrite(LED_BLUE, blue);
}

void setStatus() {
  state = map(airCondition, lowest, MAX_LIGHT, 0, 3);
  if (airCondition > MAX_PIEP)
    state = 5;
  else if (airCondition > MAX_BLINK)
    state = 4;

  if (state < 0)
    state = 0;
  if (state > 5)
    state = 5;
}



//Display

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

void checkState() {
  if (lastState) {
    drawBorder(0, 0, DISPLAY_LENGTH, DISPLAY_WIDTH, GRAPH_BACKGROUND_COLOR);
    display.fillRect(0, DATABOX_TOP_HIGHT, DISPLAY_LENGTH, DISPLAY_WIDTH, BAR_BACKGROUND_COLOR);
    createLines();
    digitalWrite(PIEZO, LOW);
    lastState = false;
  } else if (state >= 4) {
    drawBorder(0, 0, DISPLAY_LENGTH, DISPLAY_WIDTH, RED);
    if (state == 5) {
      digitalWrite(PIEZO, HIGH);
    }
    lastState = true;
  }
  writeInfo();
}

void drawBorder(int xStart, int yStart, int xEnd, int yEnd, int color) {
  display.drawLine(xStart, yStart, xEnd, yStart, color);
  display.drawLine(xStart, yEnd, xEnd, yEnd, color);
  display.drawLine(xStart, yStart, xStart, yEnd, color);
  display.drawLine(xEnd, yStart, xEnd, yEnd, color);
}

void drawDisplay() {
  display.fillScreen(GRAPH_BACKGROUND_COLOR);
  display.fillRect(0, DATABOX_TOP_HIGHT, DISPLAY_LENGTH, DISPLAY_WIDTH, BAR_BACKGROUND_COLOR);
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

boolean getData(int data) {
  valuesGraph[counter] = data;
  counter ++;
  if (!(counter < AVERAGING_GRAPH)) {
    pixel = 0;
    for (int i = 0; i < AVERAGING_GRAPH; i++) {
      pixel = pixel + valuesGraph[i];
    }
    pixel = pixel / AVERAGING_GRAPH;
    pixel = ALPHA_GRAPH * pixel + (1 - ALPHA_GRAPH) * lastPixel;
    lastPixel = pixel;
    counter = 0;
    return (true);
  } else
    return (false);
}

void drawGraph() {
  for (short x = DISPLAY_LENGTH; x > 0; x--) {
    byte arrayDigit = DISPLAY_LENGTH - x;
    if (graphData[arrayDigit] < DATABOX_TOP_HIGHT)
      display.drawPixel(x, graphData[arrayDigit], GRAPH_COLOR);
    if (x != DISPLAY_LENGTH) {
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
      display.drawPixel(x + 1, y, GRAPH_BACKGROUND_COLOR);
      display.drawPixel(x, y, GRAPH_COLOR);
    }
  }
}

// Künstliches Auffüllen der Werte, wird später vom Modul übernommen
void fillData(int data) {
  for (short x = DISPLAY_LENGTH - 1; x > 0; x--) {
    graphData[x] = graphData[x - 1];
  }
  graphData[0] = DISPLAY_WIDTH - data;
}

void writeInfo() {
  //acertain ppm color
  int currentColor = BLACK;
  switch (state) {
    case 0: currentColor = PPM_COLOR_N;
      break;
    case 1: currentColor = PPM_COLOR_R;
      break;
    case 2: currentColor = ORANGE;
      break;
    case 3: currentColor = PPM_COLOR_A;
      break;
    default: currentColor = PPM_COLOR_A;
      break;
  }

  //ppm zeichnen
  if (ventilating)
    display.fillRect(0, DATABOX_TOP_HIGHT, DISPLAY_LENGTH, BAR_STRIPE_THICKNESS, CYAN);
  else
    display.fillRect(0, DATABOX_TOP_HIGHT, DISPLAY_LENGTH, BAR_STRIPE_THICKNESS, currentColor);

  //Verhindert überschreiben von "ppm"
  if (airCondition < 1000)
    dPrint(57, 118, 1.5, currentColor, "ppm");
  else
    dPrint(57, 118, 1.5, BAR_BACKGROUND_COLOR, "ppm");

  //Clear old Pixels
  dPrint(2, 105, 3, BAR_BACKGROUND_COLOR, lastAirConditionGraph);
  //write new Pixels
  dPrint(2, 105, 3, currentColor, airCondition);
  //Set new lastAirCondition
  lastAirConditionGraph = airCondition; //Setzt letzten Wert

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

// Schreibt AIRduino fix aufs Display
void writeLoadingScreenTitle() {
  dPrint(50, 35, 4, LIGHT_BLUE, "A");
  dPrint(70, 35, 4, TURKISE, "I");
  dPrint(90, 35, 4, LIME, "R");
  dPrint(35, 65, 3, GREY, "duino");
}
