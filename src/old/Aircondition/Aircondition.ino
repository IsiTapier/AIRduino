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
#define averagingPitch 5

unsigned long  airCondition;
int lastAirCondition;
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
  ini();
}

void loop() {
  meassureAirCondition();
}

void ini() {
  //Pins
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);

  //Serial
  Serial.begin(9600);                 //Starten des Seriellen Outputs
  Serial.println(1400);               //Anpassen der Skala
  Serial.println(lowest);             //Wert rurück zum Ausgangswert

  for (int i = 0; i < averagingPitch * 2; i++) {      //Array für die letzten 10 Werte auf 0 setzten
    values[i] = 0;
  }
}

void meassureAirCondition() {
  //Messung
  airCondition = 0;                                                                                               //Zurücksetzten des AirCondition value
  for (int i = 0; i < averaging; i++) {                                                                           //Vielfaches Messen für Durschnittswerte
    value = analogRead(inputGasSensor);                                                                           //Messen der Luftqualität
    if (airCondition / i + maxIncrease < value && i != 0 /*|| value + maxIncrease < airCondition / i && i != 0*/)     //Bei Werten mit zu großen Steigungen (Senkungen) wird der Durchschnittswert verwendet
      airCondition = airCondition + airCondition / i;
    else
      airCondition = airCondition + value;                                                                        //Sonst Wert dazu addieren
    delay(10);
  }
  airCondition = airCondition / averaging;                                                                        //Berechnung des Durschnittwertes der Rechnung
  lastAirCondition = airCondition;

  //Diagramm
  airCondition = airCondition - outsideValueSensor;                                                               //Grundluftqualtität wird abgezogen
  airCondition = map(airCondition, 0, 1023, outsideValuePpm, maxPpm);                                             //Wert wird auf Co2 Konzentrationen gemapped
  Serial.println(airCondition);                                                                                     //Diagramm wird gezeichnet

  //Steigung
  for (int i = averagingPitch * 2 - 1; i > 0; i--) {
    values[i] = values[i - 1];
  }
  values[0] = airCondition;
  calculatePitch();

  //lowest value
  if (ventilating && airCondition < drop)                                                                         //Einstellung des Grund-Values
    drop = airCondition;

  //LED
  if (airCondition > maxLight)                                                                                    //Werte über dem Maximalwert werden angeglichen
    airCondition = maxLight;
  if (airCondition < lowest)
    airCondition = lowest;
  led = map(airCondition, lowest, maxLight, 0, 255);                                                              //Mapping für LED
  red = led;                                                                                                      //Wertberechnung für die rote LED
  green = 255 - led;                                                                                              //Wertberechnung für die grüne LED
  if (red < 0)                                                                                                    //Werte unter 0 werden auf 0 gesetzt
    red = 0;
  if (!ventilating)
    rgb(red, green, 0);                                                                                           //LED wird auf die entsprechenden Werte gesetzt
}

//LED write function
void rgb(int red, int green, int blue) {
  analogWrite(ledRed, red);
  analogWrite(ledGreen, green);
  analogWrite(ledBlue, blue);
}

//Pitch
void calculatePitch() {
  for (int i = 0; i < averagingPitch; i++) {
    now = now + values[i];
  }
  now = now / averagingPitch;

  for (int i = averagingPitch; i < averagingPitch * 2; i++) {
    last = last + values[i];
  }
  last = last / averagingPitch;

  pitch = now - last;

  //check Ventilating
  if (pitch * -1 > maxDecrease && !ventilating) {
    //Ventilating
    ventilating = true;
    drop = airCondition;
    rgb(0, 0, 255);
  }

  if (last < now && ventilating) {
    //stopVentilating
    if (drop - lowest < maxDropIncrease)
      lowest = drop;
    ventilating = false;
  }
  last = now;
}
