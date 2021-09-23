
  #include "Arduino.h"
  #include "manager/Manager.h"
  

/*
ToDo
- settings in den guis....
- ton sensor anschließen
- ton algo
- case for ton ding

*/

/*   const char* ssid = "DESKTOP-Q7HRET5 8763";
  const char* password = "12345678";
  const char* mqtt_server = "79.231.0.86"; */
 /*  const char* ssid = "FRITZ!Box 7590 JG";
  const char* password = "4400834912335401";
  const char* mqtt_server = "192.168.178.57"; */

  //Buchse 1.16.01

  const String software_version = "8.81";
/*   const char* ssid = "AG-iOT";
  const char* password = "#Wlan4iOT#JCBS-AG!";
  const char* mqtt_server = "169.254.191.226"; */
  const char* ssid = "JCBS-Schüler";
  const char* password = "JCBS-Schueler#2021!";
  const char* mqtt_server = "192.168.2.96";
/*
   _____      _ 
  / ____|    | |
 | (___   ___| |_ _   _ _ __
  \___ \ / _ \ __| | | | '_ \
  ____) |  __/ |_| |_| | |_) |
 |_____/ \___|\__|\__,_| .__/
                       | |
                       |_|
*/

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println();
  Serial.println("SETUP:");
  Serial.println("Serial communication started at 9600 baud");
  Serial.println();
  Manager::setup();
  general::debugPriority.setValue(DEBUG);
}

/*
  _
 | |
 | |     ___   ___  _ __
 | |    / _ \ / _ \| '_ \
 | |___| (_) | (_) | |_) |
 |______\___/ \___/| .__/
                   | |
                   |_|
*/

void loop() {
  Manager::loop();
}
