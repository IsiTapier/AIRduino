#include <Adafruit_SSD1306.h>
#include <splash.h>

#include <Wire.h>
#include <Adafruit_GFX.h>


#define D_L 128
#define D_W 64
#define arrayLength D_L

#define ledRed 9
#define ledGreen 10
#define ledBlue 11
#define inputGasSensor A0
#define outsideValueSensor 80
#define outsideValuePpm 400
#define maxPpm 5000
#define maxIncrease 10
#define maxDecrease 10
#define maxDropIncrease 50
#define averaging 100
#define maxLight 1000
#define maxBlink 1100
#define maxPiep 1200
#define maxDisplayed 1400
#define averagingPitch 5
#define alpha 0.7
#define debug true

#define criticalHight map(maxLight, 0, maxDisplayed, 0, D_W)
#define minHight map(outsideValuePpm, 0, maxDisplayed, 0, D_W)

short graphData[arrayLength];
Adafruit_SSD1306 display = Adafruit_SSD1306(D_L, D_W, &Wire, -1); // Create display object

long airCondition;
int lastAirCondition = 0;
int airConditionRaw;
int value;
int lowest = 600;
int led;
int red;
int green;
int values[averagingPitch * 2];
int last;
int now;
int pitch;
int drop;
boolean ventilating = false;

void setup() {
  Serial.begin(9600); 
  initSensor();
  initDisplay();
}

void loop() {
  loopSensor();
}


//Sensor

void initSensor() {
  //Pins
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);

  //prepare Arrays
  for (int i = 0; i < averagingPitch * 2; i++) {
    values[i] = 0;
  }
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
  if(debug) {
     Serial.println("//////////////////////////////////////");
     Serial.println("Sensor");
     Serial.println("//////////////////////////////////////");
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
     Serial.print("LED: ");
     Serial.println(led);
     Serial.println("//////////////////////////////////////");
  }
}

void meassureAirCondition() {
  //Messung
  airCondition = 0;
  for (int i = 0; i < averaging; i++) {
    value = analogRead(inputGasSensor);
    
    //Fehlmessungen überschreiben
    if(lastAirCondition + maxIncrease < value && i == 0)
      airCondition = airCondition + lastAirCondition;
    else if (airCondition / i + maxIncrease < value && i != 0)
      airCondition = airCondition + airCondition / i;
    else
      airCondition = airCondition + value;
      
    delay(10);
  }
  airCondition = airCondition / averaging;
  airConditionRaw = airCondition;
  
  //Wert smoothen;
  airCondition = alpha * airCondition + (1-alpha) * lastAirCondition;

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
  if (airCondition > maxLight)
    airCondition = maxLight;
  if (airCondition < lowest)
    airCondition = lowest;

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
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(2000);
  display.clearDisplay();

  //Auffüllen des Arrays
  for (short x = 0; x < arrayLength; x++) {
    graphData[x] = 0;
  }

}

void createLines() {
  drawLineMax();
  drawLineMin();
  display.display();
}

void drawLineMax() {
  for (short x = arrayLength; x >= 0; x--) {
    if ((x % 5) == 0) {
      display.drawPixel(x, criticalHight, WHITE);
    }
  }
}

void drawLineMin() {
  for (short x = arrayLength; x >= 0; x--) {
    if ((x % 5) == 0) {
      display.drawPixel(x, minHight, WHITE);
    }
  }
}

void draw(int data) {
  display.clearDisplay();
  fillData(data);
  drawGraph();
  createLines();
  //writeVar(); //verdrehte Schrift
}

void drawGraph() {
  for (short x = 0; x < arrayLength; x++) {
    display.drawPixel(x, graphData[x], WHITE);

    if (x != 0) {
      if (graphData[x - 1] < graphData[x]) {
        for (byte y = graphData[x]; y > graphData[x - 1]; y--) {
          if (display.getPixel(graphData[x - 1], y) == 0) {
            display.drawPixel(x, y, WHITE);
          }
        }
      }
      if (graphData[x - 1] > graphData[x]) {
        for (byte y = graphData[x]; y < graphData[x - 1]; y++) {
          //if (display.getPixel(graphData[x - 1], y) == 0) {
          display.drawPixel(x, y, WHITE);
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
