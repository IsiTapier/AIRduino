#include "SD_Card_Extension.h"


#include "FS.h"
#include "SD.h"
#include "SPI.h"

SD_Card_Extension::SD_Card_Extension(int CSPin, const String filename)
: _filename(filename),
  _CSPin(CSPin),
  _initialized(false) {
}

void SD_Card_Extension::initialize() {
  if(_initialized == false){
    pinMode(_CSPin, OUTPUT);
    if (!SD.begin(_CSPin)) {
      Serial.println("Card failed, or not present");
      return;
    }
    Serial.println("card initialized.");
    _initialized = true;
  }
}

bool SD_Card_Extension::saveValuesToSD(int value1, int value2, int value3, int value4) {
  String dataString = String(value1)+ "," + String(value2) + "," + String(value3) + "," + String(value4); // convert to CSV (comma seperated values)
  return saveData(dataString);
}

bool SD_Card_Extension::saveData(const String & dataString) {
  initialize();
  if(SD.exists(_filename)) {
    File sensorData = SD.open(_filename, FILE_WRITE);
    if (sensorData) {
      sensorData.println(dataString); //print data to File
      sensorData.close(); // close the file
      return true;
    }
  } else {
    Serial.print("Error writing to file "); Serial.println(_filename);
    return false;
  }
}
