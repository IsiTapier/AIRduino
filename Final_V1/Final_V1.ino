#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>


#define D_L 160
#define D_W 128
#define arrayLength verticalLine

#define delayTime 2*1000

#define TFT_CS     10
#define TFT_RST    9  // You can also connect this to the Arduino reset in which case, set this #define pin to -1!
#define TFT_DC     8
#define rotation -45

#define ledRed 5
#define ledGreen 6
#define ledBlue 7
#define inputGasSensor A0
#define outsideValueSensor 80
#define outsideValuePpm 400
#define maxPpm 5000
#define maxIncrease 1.5
#define maxDecrease 10
#define maxDropIncrease 50
#define averaging 100
#define maxLight 1000
#define maxBlink 1100
#define maxPiep 1200
#define maxDisplayed 1400
#define averagingPitch 5
#define alpha 0.7


#define verticalLine D_L / 2
#define criticalHight 160 - map(maxLight, 0, maxDisplayed, 0, D_W)
#define minHight 160 - map(outsideValuePpm, 0, maxDisplayed, 0, D_W)
#define DisplayBottomBorder 100

//variabel defines:
#define ROTATION 45
#define DEBUG true

#define STAGE_TIME 1*1000
#define VENTILATING_TIMEOUT 5*1000

#define AVERAGING_MEASUREMENTS 100
#define AVERAGING_GRADIENT 5
#define ALPHA_MEASUREMENTS 0.7
#define ALPHA_LOWEST 0.5

#define OSV_SENSOR 80
#define OSV_PPM 400
#define MIN_HIGHT DISPLAY_WIDTH - map(OSV_PPM, 0, MAX_DISPLAYED_PPM, 0, DISPLAY_WIDTH)
#define CRITICAL_HIGHT DISPLAY_WIDTH - map(MAX_LIGHT, 0, MAX_DISPLAYED_PPM, 0, DISPLAY_WIDTH)

#define MAX_SENSOR 1023
#define MAX_PPM 5000
#define MAX_INCREASE 1.5
#define MAX_DECREASE 10
#define MAX_DROP_INCREASE 50
#define MAX_LIGHT 1000
#define MAX_BLINK 1100
#define MAX_PIEP 1200
#define MAX_DISPLAYED_PPM 1280

//pins:
#define TFT_CS     10
#define TFT_RST    9
#define TFT_DC     8
#define LED_RED 5
#define LED_GREEN 6
#define LED_BLUE 7
#define GAS_SENSOR A0

//Data:
#define DISPLAY_LENGTH 159
#define DISPLAY_WIDTH 127

//Design
#define DATABOX_TOP_HIGHT 100
#define GRAPH_COLOR WHITE//MAGENTA
#define GRAPH_BACKGROUND_COLOR BLACK
#define BAR_BACKGROUND_COLOR GREY
#define BAR_STRIPE_THICKNESS 3


// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define GREY 0x8C51
#define LIME 0x87F4


#define PPM_COLOR_N WHITE //Normal
#define PPM_COLOR_A 0xF800 //Alarm

//Customize
#define GraphColor WHITE
#define GraphBackgroundColor BLACK
#define BarBackgroundColor BLACK
#define BarStripeThickness 3
#define BarStripeColor WHITE


short graphData[arrayLength];

Adafruit_ST7735 display = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

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
short airStatus;
short lastAirConditionGraph = 0;


byte STATUS = 1; //Normal or Risk or Alarm

void setup() {

  Serial.begin(9600);
  initDisplay();
  initSensor();
}

void loop() {
  loopSensor();

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

  airConditionRaw = analogRead(GAS_SENSOR);
  lastAirCondition = analogRead(GAS_SENSOR);
  startTime = millis();
}

void loopSensor() {
  meassureAirCondition();
  mapAirCondition();
  calculatePitch();
  checkVentilating();
  setStatus();
  writeLed();
  draw(map(airCondition, 0, MAX_DISPLAYED_PPM, 0, DISPLAY_WIDTH));
  debugSensor();
}

void debugSensor() {
  if (DEBUG) {
    Serial.println("");
    Serial.println("Sensor");
    Serial.println("");
    Serial.print("Analog: ");
    Serial.println(analogRead(inputGasSensor));
    Serial.print("Average: ");
    Serial.println(airConditionRaw);
    Serial.print("Smoothed: ");
    Serial.println(lastAirCondition);
    Serial.print("PPM: ");
    Serial.println(airCondition);
    Serial.print("Pitch: ");
    Serial.println(pitch);
    Serial.print("Lowest: ");
    Serial.println(lowest);
    debug("Analog", analogRead(GAS_SENSOR));
    debug("Average", airConditionRaw);
    debug("Smoothed", lastAirCondition);
    debug("PPM", airCondition);
    debug("Pitch", pitch);
    debug("Lowest", lowest);
    debug("Status", airStatus);

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


    delay(delayTime / averaging);
  }

  airCondition = airCondition / averaging;

  delay(STAGE_TIME / AVERAGING_MEASUREMENTS);


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

  airCondition = airCondition - OSV_SENSOR;
  airCondition = map(airCondition, 0, MAX_SENSOR, OSV_PPM, MAX_PPM);

  //to Graph
  draw(map(airCondition, 0, maxDisplayed, 0, DisplayBottomBorder));

}

void calculatePitch() {
  //store last AirConditions
  for (int i = averagingPitch * 2 - 1; i > 0; i--) {
    for (int i = AVERAGING_GRADIENT * 2 - 1; i > 0; i--) {

      values[i] = values[i - 1];
    }
    values[0] = airCondition;


    //average current AirCondition
    for (int i = 0; i < averagingPitch; i++) {
      now = now + values[i];
    }
    now = now / averagingPitch;

    //average last AirCondition
    for (int i = averagingPitch; i < averagingPitch * 2; i++) {
      last = last + values[i];
    }
    last = last / averagingPitch;
    //average AirConditions
    now = average(values, 0, AVERAGING_GRADIENT);
    last = average(values, AVERAGING_GRADIENT, AVERAGING_GRADIENT * 2);


    //Pitch
    pitch = now - last;
  }
}

void checkVentilating() {
  //lowest value
  if (ventilating && airCondition < drop) {
    drop = airCondition;
  }
  //start Ventilating
  if (pitch * -1 > maxDecrease && !ventilating) {

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
  //prepare Values
  /*if (airCondition > maxLight)
    airCondition = maxLight;
    if (airCondition < lowest)
    airCondition = lowest;*/

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
  airStatus = map(airCondition, lowest, MAX_LIGHT, 0, 2);
  if (airCondition > MAX_PIEP)
    airStatus = 4;
  else if (airCondition > MAX_BLINK)
    airStatus = 3;

  if (airStatus < 0)
    airStatus = 0;
  if (airStatus > 4)
    airStatus = 4;
}



//Display

void initDisplay() {
  display.initR(INITR_BLACKTAB);  // Initialize a ST7735S chip, black tab
  display.setTextWrap(false);  // By default, long lines of text are set to automatically “wrap” back to the leftmost column.
  // To override this behavior (so text will run off the right side of the display - useful for
  // scrolling marquee effects), use setTextWrap(false). The normal wrapping behavior is restored
  // with setTextWrap(true).
  display.setRotation(rotation);
  display.fillScreen(GraphBackgroundColor);

  //Auffüllen des Arrays
  for (short x = 0; x < arrayLength; x++) {
    graphData[x] = D_W - 0;
    display.initR(INITR_BLACKTAB);
    display.setTextWrap(false);
    display.setRotation(ROTATION);
    display.fillScreen(GRAPH_BACKGROUND_COLOR);

    //Auffüllen des Arrays
    for (short x = 0; x < DISPLAY_LENGTH; x++) {
      graphData[x] = DISPLAY_LENGTH;
    }
    graphData[0] = DATABOX_TOP_HIGHT - 1;

    // Draw Bar Background
    display.fillRect(0, DisplayBottomBorder + 1, D_L, D_W, BarBackgroundColor);
    display.fillRect(0, DisplayBottomBorder + 1, D_L, BarStripeThickness, BarStripeColor);
    Serial.println("Come on");

  }
}


void drawLineMin() {
  for (short x = D_L; x >= 0; x--) {
    if ((x % 5) == 0) {
      display.drawPixel(x, minHight, WHITE);
    }
    display.fillRect(0, DATABOX_TOP_HIGHT + 1, DISPLAY_LENGTH, DISPLAY_WIDTH, BAR_BACKGROUND_COLOR);
  }
}

void createLines() {
  drawLine(DISPLAY_LENGTH + 1, CRITICAL_HIGHT, 5);
  drawLine(DISPLAY_LENGTH + 1, MIN_HIGHT, 5);
  writeInfo();
}

void drawLine(int x, int y, int z) {
  for (int i = 0; i < x; i = i + z) {
    display.drawPixel(i, y, WHITE);
  }
}

void draw(int data) {

  createLines();
  fillData(data);
  drawGraph();
}


void drawGraph() {
  byte BorderDifference = D_W - DisplayBottomBorder;
  for (short x = verticalLine; x > 0; x--) {
    byte arrayDigit = verticalLine - x;

    display.drawPixel(x, graphData[arrayDigit] - BorderDifference, GraphColor);
    if (x != verticalLine) {
      if (graphData[arrayDigit - 1] < graphData[arrayDigit]) {
        for (byte y = graphData[arrayDigit]; y > graphData[arrayDigit - 1]; y--) {
          //if (display.getPixel(graphData[x - 1], y) == 0) {
          display.drawPixel(x, y - BorderDifference, GraphColor);
          display.drawPixel(x + 1, y - BorderDifference, GraphBackgroundColor);
          //}
        }
      }
      for (byte y = graphData[arrayDigit]; y < graphData[arrayDigit - 1]; y++) {
        //if (display.getPixel(graphData[x - 1], y) == 0) {
        display.drawPixel(x, y - BorderDifference, GraphColor);
        display.drawPixel(x + 1, y - BorderDifference, GraphBackgroundColor);
        //}
      }
    }
  }
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
  switch (airStatus) {
    case 0: currentColor = GREEN;//PPM_COLOR_N;
      break;
    case 1: currentColor = YELLOW; //PPM_COLOR_R;
      break;
    case 2: currentColor = RED; //PPM_COLOR_A;
      break;
    default: currentColor = RED;
      break;
  }

  //ppm zeichnen
  if (ventilating)
    display.fillRect(0, DATABOX_TOP_HIGHT + 1, DISPLAY_LENGTH, BAR_STRIPE_THICKNESS, CYAN);
  else
    display.fillRect(0, DATABOX_TOP_HIGHT + 1, DISPLAY_LENGTH, BAR_STRIPE_THICKNESS, currentColor);

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
  dPrint(90, 110, 2, BAR_BACKGROUND_COLOR, lastTime);
  //write new Pixels
  if (minutes >= 20)
    dPrint(90, 110, 2, RED, Time);
  else
    dPrint(90, 110, 2, BLACK, Time);
  //Set new lastAirCondition
  lastTime = Time; //Setzt letzten Wert
}

void dPrint(int y, int x, float scale, int color, String text) {
  display.setCursor(y, x);
  display.setTextSize(scale);
  display.setTextColor(color);
  display.println(text);
}

void dPrint(int y, int x, float scale, int color, int text) {
  display.setCursor(y, x);
  display.setTextSize(scale);
  display.setTextColor(color);
  display.println(text);
}
