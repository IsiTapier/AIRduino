

  #include "src/display/Display.h"
  


//                                      ,,               ,,
//      db      `7MMF'`7MM"""Mq.      `7MM               db
//     ;MM:       MM    MM   `MM.       MM
//    ,V^MM.      MM    MM   ,M9   ,M""bMM `7MM  `7MM  `7MM  `7MMpMMMb.  ,pW"Wq.
//   ,M  `MM      MM    MMmmdM9  ,AP    MM   MM    MM    MM    MM    MM 6W'   `Wb
//   AbmmmqMA     MM    MM  YM.  8MI    MM   MM    MM    MM    MM    MM 8M     M8
//  A'     VML    MM    MM   `Mb.`Mb    MM   MM    MM    MM    MM    MM YA.   ,A9
//.AMA.   .AMMA..JMML..JMML. .JMM.`Wbmd"MML. `Mbod"YML..JMML..JMML  JMML.`Ybmd9'
//


  const char* ssid = "DESKTOP-Q7HRET5 8763";
  const char* password = "12345678";
  const char* mqtt_server = "87.143.106.137";
 /* const char* ssid = "FRITZ!Box 7590 JG";
  const char* password = "4400834912335401";
  const char* mqtt_server = "192.168.178.57";
 /*const char* ssid = "AG-iOT";
    const char* password = "#Wlan4iOT#JCBS-AG!";
    const char* mqtt_server = "192.168.178.57";*/



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
  Serial.println();
  Serial.println();
  Serial.println("SETUP:");
  Serial.println("Serial communication started at 9600 baud");
  Serial.println();
  Display::setup();
  general::debugPriority.setValue(INFO);
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
  Display::loop();
}
