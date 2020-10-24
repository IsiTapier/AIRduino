#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define D_L 128 //Display Length // Just for initializing the screen
#define D_W 64 //Display Width

//DivideLines
#define criticalHight 48
#define minHight 16

#define maxValue 2000
#define lowValue 0

#define updatePeriod 700 // in ms

const short arrayLength = D_L;

long timestamp = 0;

short graphData[arrayLength];
short sortValues[arrayLength];

Adafruit_SSD1306 display = Adafruit_SSD1306(D_L, D_W, &Wire, -1); // Create display object

void setup() {
  Serial.begin(115200);


  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(2000);
  display.clearDisplay();

  //Auffüllen des Arrays
  Serial.println("Array: ");
  for (short x = 0; x < arrayLength; x++) {
    graphData[x] = 0;
    Serial.println(graphData[x]);
  }


}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(graphData[arrayLength - 1]);
  if (millis() >= timestamp) {
    timestamp = millis() + updatePeriod;
    draw();
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

void draw() {
  display.clearDisplay();
  fillData();
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


void fillData() {  // Künstliches Auffüllen der Werte, wird später vom Modul übernommen
  for (short x = arrayLength - 1; x > 0; x--) {
    graphData[x] = graphData[x - 1];      
  }
  graphData[0] = getData();
}

short getData() {
  byte z = random(maxValue + 1);
  return z;
}
