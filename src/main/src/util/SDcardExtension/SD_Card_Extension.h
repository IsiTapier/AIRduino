/*
* SD card attached to SPI bus as follows (names after the backslashs are the names of the Airduino Display SD_Card Module Pins):
** MOSI - pin 11  /DI  (51 MEGA)
** MISO - pin 12  /DO (50 MEGA)
** CLK - pin 13   /SCK (52 MEGA)
** CS - pin 10    /SS  (53 on MEGA)
*/
#ifndef SD_Card_Extension_H
#define SD_Card_Extension_H

 #include "Arduino.h"

class SD_Card_Extension {

public:
  SD_Card_Extension(int CSPin, String filename);   //Uno 10  for filename please chose sth. like "filename.csv"

  bool saveValuesToSD(int value1, int value2, int value3, int value4); //insert values to save. If SD Card or File is not working function returns false

private:
  bool saveData(const String & dataString);
  void initialize();

  bool _initialized;
  const String _filename;
  int _CSPin;
};
#endif
