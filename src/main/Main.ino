extern "C" {
  #include "src/meassure/meassure.h"
  #include "src/display/displayV1/DisplayV1.h"
  #include "src/display/displayV2/DisplayV2.h"
}



//                                      ,,               ,,
//      db      `7MMF'`7MM"""Mq.      `7MM               db
//     ;MM:       MM    MM   `MM.       MM
//    ,V^MM.      MM    MM   ,M9   ,M""bMM `7MM  `7MM  `7MM  `7MMpMMMb.  ,pW"Wq.
//   ,M  `MM      MM    MMmmdM9  ,AP    MM   MM    MM    MM    MM    MM 6W'   `Wb
//   AbmmmqMA     MM    MM  YM.  8MI    MM   MM    MM    MM    MM    MM 8M     M8
//  A'     VML    MM    MM   `Mb.`Mb    MM   MM    MM    MM    MM    MM YA.   ,A9
//.AMA.   .AMMA..JMML..JMML. .JMM.`Wbmd"MML. `Mbod"YML..JMML..JMML  JMML.`Ybmd9'
//



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
  //DisplayV1::setup();
  DisplayV2::setup();
  Meassure::setup();
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
  Meassure::loop();
  //DisplayV1::loop();
  DisplayV2::loop();
}
