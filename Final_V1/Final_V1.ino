#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define D_L 160
#define D_W 128
#define arrayLength verticalLine

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
#define debug false
#define delayTime 0.5*1000

#define verticalLine D_L / 2
#define criticalHight map(maxLight, 0, maxDisplayed, 0, D_W)
#define minHight map(outsideValuePpm, 0, maxDisplayed, 0, D_W)

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


short graphData[arrayLength];

Adafruit_ST7735 display = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

int airCondition = 0;
short lastAirCondition = 0;
short airConditionRaw = 0;
short value;
short lowest = 600;
short led;
short red;
short green;
short values[averagingPitch * 2];
short last;
short now;
short pitch;
short drop;
boolean ventilating = false;

short count = 0;

void setup() {

  Serial.begin(9600);
  initDisplay();
  initSensor();
}

void loop() {
  loopSensor();
  count++;
}


//Sensor

void initSensor() {
  //Pins
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);

  airConditionRaw = analogRead(inputGasSensor);
  lastAirCondition = analogRead(inputGasSensor);
}

void loopSensor() {
  meassureAirCondition();
  mapAirCondition();
  calculatePitch();
  checkVentilating();
  writeLed();
  debugSensor();
}

void debugSensor() {
  if (debug) {
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
  }
}

void meassureAirCondition() {
  //Messung
  airCondition = 0;
  for (int i = 0; i < averaging; i++) {
    value = analogRead(inputGasSensor);

    //Fehlmessungen überschreiben
    if (airConditionRaw * maxIncrease < value && i == 0)
      airCondition = airCondition + airConditionRaw;
    else if (airCondition / i * maxIncrease < value && i != 0)
      airCondition = airCondition + airCondition / i;
    else
      airCondition = airCondition + value;

    delay(delayTime / averaging);
  }

  airCondition = airCondition / averaging;
  airConditionRaw = airCondition;

  //Wert smoothen;
  airCondition = alpha * airCondition + (1 - alpha) * lastAirCondition;

  lastAirCondition = airCondition;

}

void mapAirCondition() {
  //to PPM
  airCondition = airCondition - outsideValueSensor;
  airCondition = map(airCondition, 0, 1023, outsideValuePpm, maxPpm);

  //to Graph
  draw(map(airCondition, 0, maxDisplayed, 0, D_W));
}

void calculatePitch() {
  //store last AirConditions
  for (int i = averagingPitch * 2 - 1; i > 0; i--) {
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

  //Pitch
  pitch = now - last;
}

void checkVentilating() {
  //lowest value
  if (ventilating && airCondition < drop)
    drop = airCondition;

  //start Ventilating
  if (pitch * -1 > maxDecrease && !ventilating) {
    ventilating = true;
    drop = airCondition;
    rgb(0, 0, 255);
  }

  //stop Ventilating
  if (last < now && ventilating) {
    if (drop - lowest < maxDropIncrease)
      lowest = drop;
    ventilating = false;
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
  led = map(airCondition, lowest, maxLight, 0, 255);
  red = led;
  green = 255 - led;

  //turn on
  if (!ventilating)
    rgb(red, green, 0);
}

void rgb(int red, int green, int blue) {
  analogWrite(ledRed, red);
  analogWrite(ledGreen, green);
  analogWrite(ledBlue, blue);
}



//Display

void initDisplay() {
  display.initR(INITR_BLACKTAB);  // Initialize a ST7735S chip, black tab
  display.fillScreen(ST7735_BLACK);  // Fill screen with black
  display.setTextWrap(false);  // By default, long lines of text are set to automatically “wrap” back to the leftmost column.
  // To override this behavior (so text will run off the right side of the display - useful for
  // scrolling marquee effects), use setTextWrap(false). The normal wrapping behavior is restored
  // with setTextWrap(true).
  display.setRotation(rotation);
  display.fillScreen(ST7735_BLACK);

  //Auffüllen des Arrays
  for (short x = 0; x < arrayLength; x++) {
    graphData[x] = 0;
  }

}

void createLines() {
  drawLineVertical();
  drawLineMax();
  drawLineMin();
}

void drawLineVertical() {
  for(byte y = 0; y <= D_W; y++) {
    display.drawPixel(verticalLine, y, GREY);
  }
}

void drawLineMax() {
  for (short x = D_L; x >= 0; x--) {
    if ((x % 5) == 0) {
      display.drawPixel(x, criticalHight, WHITE);
    }
  }
}

void drawLineMin() {
  for (short x = D_L; x >= 0; x--) {
    if ((x % 5) == 0) {
      display.drawPixel(x, minHight, WHITE);
    }
  }
}

void draw(int data) {
  
  fillData(data);
  drawGraph();
  createLines();
  
}


void drawGraph() {
  for (short x = verticalLine; x < D_L; x++) {
    byte arrayDigit = x - verticalLine;
    if(graphData[arrayDigit] != 0) display.drawPixel(x, graphData[arrayDigit], WHITE);
    
    Serial.print("Array Stelle: ");
    Serial.println(arrayDigit);
    if (x != verticalLine) {
      if (graphData[arrayDigit - 1] < graphData[arrayDigit]) {
        for (byte y = graphData[arrayDigit]; y > graphData[arrayDigit - 1]; y--) {
          //if (display.getPixel(graphData[x - 1], y) == 0) {
          display.drawPixel(x, y, WHITE);
          display.drawPixel(x - 1, y, BLACK);
          //}
        }
      }
      if (graphData[arrayDigit - 1] > graphData[arrayDigit]) {
        for (byte y = graphData[arrayDigit]; y < graphData[arrayDigit - 1]; y++) {
          //if (display.getPixel(graphData[x - 1], y) == 0) {
          display.drawPixel(x, y, WHITE);
          display.drawPixel(x - 1, y, BLACK);
          //}
        }
      }
    } 

  }
}

void fillData(int data) {  // Künstliches Auffüllen der Werte, wird später vom Modul übernommen
  for (short x = arrayLength - 1; x > 0; x--) {
    graphData[x] = graphData[x - 1];
  }
  graphData[0] = data;
}
