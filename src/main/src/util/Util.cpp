/*
  Util.cpp - Util-Library for Librarys.
*/

#include "Arduino.h"
#include "Util.h"

  TFT_eSPI display(DISPLAY_HEIGHT, DISPLAY_LENGTH);
  TouchScreen ts(TFT_D1, TFT_DC, TFT_CS, TFT_D0, TFT_D1, 33, TFT_CS, TFT_D0, 26, TOUCH_RESISTANCE);

  Mode mode = LOADINGSCREEN;
  Mode lastMode;
  Version lastVersion;

  Calibration calibration[] = {
      Calibration(189, 216, 400, 975),//(449, 714, 576, 1184), //576 1424 496 478 //216 875 //189 400
      Calibration(710, 784, 550, 900),
      Calibration(40, 60, 400, 1000)
  };

  //State
  State getStateOf(int value) {
         if (value <= LIMIT_GOOD)
      return((State) GOOD);
    else if (value <= LIMIT_MEDIUM)
      return((State) MEDIUM);
    else if (value < LIMIT_BLINK)
      return((State) BAD);
    else if (value >= LIMIT_BLINK)
      return((State) BLINK);
    else if (value >= LIMIT_PIEP)
      return((State) PIEP);
    else
      return((State) BAD);
  }

  /*long map(double x, long in_min, long in_max, long out_min, long out_max) {
    return round((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
  }*/

  int map(int value, int startA, int startB, float factor) {
    value = value - startA;
    value = value * factor;
    value = value + startB;
    return(value);
  }

  int average(int averageArray[], int averageStart, int averageEnd) {
    int sum = 0;
    for (int i = averageStart; i < averageEnd; i++) {
      sum = sum + averageArray[i];
    }
    return (sum / (averageEnd - averageStart));
  }

  void drawBorder(int x, int y, int length, int height, int thickness, int color) {
    length -= thickness;
    height -= thickness;
    for(int i = 0; i < thickness; i++) {
      for(int i = 0; i < thickness; i++) {
        display.drawLine(x, y, x+length, y, color);
        display.drawLine(x, y+height, x+length, y+height, color);
        display.drawLine(x, y, x, y+height, color);
        display.drawLine(x+length, y, x+length, y+height, color);
        x++;
      }
      y++;
      x -= thickness;
    }
  }

  void drawLine(int x, int y, int length, int height, int color, int a, int b, boolean filter1, int filtervalue1, boolean filter2, State filtervalue2) {
    length += x;
    height += y;
    if(a > 1)
      x += floor((length-1-x)%a/2);
    if(b > 1)
      y += floor((height-1-y)%b/2);
    for (int i = x; i < length; i+=a) {
      for (int j = y; j < height; j+=b) {
        if(filter1) {
          if(display.readPixel(i, j) == (0x0FFFU * BACKGROUND_COLOR)/100) {
            if(filter2 && filtervalue1 >= DISPLAYED_PPM_HIGHEST)
              display.drawPixel(i, j, filtervalue2.getColor(COLORED_BAR));
            else
              display.drawPixel(i, j, color);
          }
        } else
          display.drawPixel(i, j, color);
      }
    }
  }

  void showBoxes() {
    display.fillRect(PPM_MARGIN_LEFT, DISPLAY_HEIGHT - PPM_HEIGHT, PPM_LENGTH, PPM_HEIGHT, GREEN);
    display.fillRect(DISPLAY_LENGTH - TIMER_LENGTH - TIMER_MARGIN_RIGHT, DISPLAY_HEIGHT - TIMER_MARGIN_BOTTOM - TIMER_HEIGHT, TIMER_LENGTH, TIMER_HEIGHT, WHITE);
    display.fillRect(DISPLAY_LENGTH - TIMER_LENGTH - TIMER_MARGIN_RIGHT, DISPLAY_HEIGHT - TIMER_MARGIN_BOTTOM - TIMER_SIZE*LETTER_HEIGHT, TIMER_LENGTH, TIMER_SIZE*LETTER_HEIGHT, BLUE);
    display.fillRect((DISPLAY_LENGTH - STATUS_LENGTH)/2, STATUS_MARGIN_TOP, STATUS_LENGTH, STATUS_HEIGHT, RED);
  }

  void dPrint(String text, int x, int y, int size, int color, int datum, int backgroundColor, String oldText, int oldTextSize, int padding) {
    display.setTextPadding(padding);
    display.setTextDatum(datum);
    if(backgroundColor == 0)
      backgroundColor = BACKGROUND_COLOR;
    if(oldText.length() != 0) {
      if(oldTextSize < 0)
        oldTextSize = size;
      int xold = x;
      if(datum % 3 == 1) {
        x -= size*LETTER_LENGTH*text.length()/2;
        xold -= oldTextSize*LETTER_LENGTH*oldText.length()/2;
        display.setTextDatum(datum-1);
      } else if(datum % 3 == 2) {
        x -= LETTER_LENGTH*size*text.length();
        xold -= LETTER_LENGTH*oldTextSize*oldText.length();
        display.setTextDatum(datum-2);
      }
      display.setTextColor(backgroundColor);
      display.setTextSize(oldTextSize);
      for(int i = 0; i < oldText.length(); i++) {
        if(text.charAt(i) != oldText.charAt(i) || text.length() != oldText.length()) {
          display.drawString((String) oldText.charAt(i), (xold+LETTER_LENGTH*oldTextSize*i), y);
        }
      }
      display.setTextColor(color);
      display.setTextSize(size);
      for(int i = 0; i < text.length(); i++) {
        if(text.charAt(i) != oldText.charAt(i) || text.length() != oldText.length()) {
          display.drawString((String) text.charAt(i), x+LETTER_LENGTH*size*i, y);
        }
      }
    } else {
      display.setTextSize(size);
      display.setCursor(x, y);
      if(backgroundColor > 0)
        display.setTextColor(color, backgroundColor);
      else
        display.setTextColor(color);
      display.drawString(text);
    }
  }

  //Verkürzung: Writing mit Integern
  void dPrint(int text, int x, int y, int size, int color, int datum, int backgroundColor, int oldText, int oldTextSize, int padding) {
    dPrint(String(text), x, y, size, color, datum, backgroundColor, (oldText == -1) ? "" : String(oldText), oldTextSize, padding);
  }

  //Loading Screen
  void loadingScreen() {
    debug(DEBUG, SETUP, "loadingscreen started");
    display.fillScreen(BACKGROUND_COLOR);
    writeLoadingScreenTitle();

    debug(DEBUG, SETUP, "loadingscreen ended");
  }

  void writeLoadingScreenTitle() {
    short distanceToFirstLetterSub = 52;
    short distanceToFirstDot = 90;
    short DotVerticalPosition = 155;
    byte DD = DD_MARGIN; //3d Versatz


    dPrint("duino", distanceToFirstLetterSub + 2, 120 + 2, LOADING_SCREEN_SUB_SIZE, DARKGREY);
    dPrint("duino", distanceToFirstLetterSub, 120, LOADING_SCREEN_SUB_SIZE, GREY);

    //Draw: Loading Dots
    byte c = 0;
    for (int x = LOADING_SCREEN_TIME * 2; x >= 0; x--) {
      c++;
      dPrint("...", distanceToFirstDot, DotVerticalPosition, LOADING_SCREEN_DOT_SIZE, BACKGROUND_COLOR); // Altes Clearen
      dPrint("...", distanceToFirstDot + DD, DotVerticalPosition + DD, LOADING_SCREEN_DOT_SIZE, BACKGROUND_COLOR);
      switch (c) {
        case 1:
            dPrint(".", distanceToFirstDot + DD, DotVerticalPosition + DD, LOADING_SCREEN_DOT_SIZE, GREY);
            dPrint(".",distanceToFirstDot, DotVerticalPosition, LOADING_SCREEN_DOT_SIZE, LOADING_SCREEN_DOTS_COLOR);

            writeLoadingAnimation(LIGHT_BLUE, TURKISE, LIME);
          break;
        case 2:
            dPrint("..", distanceToFirstDot + DD, DotVerticalPosition + DD, LOADING_SCREEN_DOT_SIZE, GREY);
            dPrint("..", distanceToFirstDot, DotVerticalPosition, LOADING_SCREEN_DOT_SIZE, LOADING_SCREEN_DOTS_COLOR);

            writeLoadingAnimation(TURKISE, LIME, LIGHT_BLUE);
          break;
        case 3:
          dPrint("...", distanceToFirstDot + DD, DotVerticalPosition + DD, LOADING_SCREEN_DOT_SIZE, GREY);
          dPrint("...", distanceToFirstDot, DotVerticalPosition, LOADING_SCREEN_DOT_SIZE, LOADING_SCREEN_DOTS_COLOR);

          writeLoadingAnimation(LIME, LIGHT_BLUE, TURKISE);
          c = 0;
          break;
      }
      delay(500);
    }

  }

  void writeLoadingAnimation(int c1, int c2, int c3) {
    long endTime = millis() + LOADING_SCREEN_TIME * 1000;

    short distanceToFirstLetter = 70 + DD_MARGIN;
    short VerticalDistanceToFirstLetter = 55 + DD_MARGIN;
    dPrint("A", distanceToFirstLetter, VerticalDistanceToFirstLetter, LOADING_SCREEN_TITLE_SIZE, GREY);
    dPrint("I", distanceToFirstLetter + 60, VerticalDistanceToFirstLetter, LOADING_SCREEN_TITLE_SIZE, GREY);
    dPrint("R", distanceToFirstLetter + 120, VerticalDistanceToFirstLetter, LOADING_SCREEN_TITLE_SIZE, GREY);

    distanceToFirstLetter = distanceToFirstLetter - DD_MARGIN;
    VerticalDistanceToFirstLetter = VerticalDistanceToFirstLetter - DD_MARGIN;
    dPrint("A", distanceToFirstLetter, 55, LOADING_SCREEN_TITLE_SIZE, c1);
    dPrint("I", distanceToFirstLetter + 60, 55, LOADING_SCREEN_TITLE_SIZE, c2);
    dPrint("R", distanceToFirstLetter + 120, 55, LOADING_SCREEN_TITLE_SIZE, c3);
  }

  //Database connection
  unsigned long lastMsg = 0;
  char msg[MSG_BUFFER_SIZE];
  int value = 0;
  String device_grade;

  WiFiClient espClient;
  PubSubClient client(espClient);
  String device_id;


  void setupDatabaseConnection() {
    getUniqueID();
    Serial.println(device_id);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    delay(500);
    //connect client the first time
    if (!client.connected()) {
      reconnect();
    }
    config_request();
    subscribeToActivityRequest();
  }

  void callback(char* topic_char, byte* payload, unsigned int length) {
    Serial.print("MQTT: (");
    Serial.print(topic_char);
    Serial.print(") ");
    Serial.print(":: ");
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();

    //get Topic as a String
    String topic = "";
    for (int i = 0; i < strlen(topic_char); i++) {
      //Serial.print((char)payload[i]);
      topic = topic + "" + (char)topic_char[i];
    }

    if (topic == "config/get/" + device_id + "/grade") {
      device_grade = "";
      for (int i = 0; i < length; i++) {
        device_grade = device_grade + "" + (char)payload[i];
      }
      if (device_grade[0] == 'a' && device_grade[1] == 'u' && device_grade[2] == 't') { //if the grade is auto generated
        debug(ERROR, DATABASE, "///////////////////// CONFIG ///////////////////////////");
        debug(ERROR, DATABASE, "Please enter the grade of your device into the database");
        debug(ERROR, DATABASE, "////////////////////////////////////////////////////////");
      }
    }

    if (topic == "config/get/" + device_id + "/version") {
      general::version.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + device_id + "/maintenance_mode") {
      //general: = atoi((char*)payload), false);
    }

    if (topic == "config/get/" + device_id + "/theme") {
      general::theme.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + device_id + "/state") {
      String language = "";
      for (int i = 0; i < length; i++) {
        language = language + "" + (char)payload[i];
      }
      if(language == "deutsch")
        general::state.setValue(0, false);
      else if(language == "englisch")
        general::state.setValue(1, false);
      else if(language == "franzoesisch")
        general::state.setValue(2, false);
      else if(language == "java")
        general::state.setValue(3, false);
      else if(language == "spanisch")
        general::state.setValue(4, false);
      else if(language == "special")
        general::state.setValue(5, false);
    }

    if (topic == "config/get/" + device_id + "/peep") {
      general::piezo.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + device_id + "/blink") {
      general::blink.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + device_id + "/graphSpeed") {
      general::graphSpeed.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + device_id + "/menu_segments") {
      general::segments.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + device_id + "/blinkThickness") {
      general::blinkThickness.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + device_id + "/ventilatingTimeout") {
      general::ventilatingTimeout.setValue((short) atoi((char*)payload), false); //int x = *payload
    }

    if (topic == "config/get/" + device_id + "/c_design") {
      colorModes::variousColors.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + device_id + "/c_chart") {
      colorModes::coloredChart.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + device_id + "/c_bar") {
      colorModes::coloredBar.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + device_id + "/c_state") {
      colorModes::coloredState.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + device_id + "/c_time") {
      colorModes::coloredTime.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + device_id + "/c_value") {
      colorModes::coloredValue.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + device_id + "/c_slider") {
      colorModes::coloredSlider.setValue((short) atoi((char*)payload), false);
    }

    //check activity

    if ("" + topic == "activity/request") {
      if ("" + topic == "activity/request") {
        String output = device_grade;
        if (general::maintenance_mode.getValue() == 0) {
          output = output + ",1";
        }
        if (general::maintenance_mode.getValue() == 1) {
          output = output + ",2";
        }
        client.publish("activity/check", output.c_str());
      }
    }

    if (topic == "maintenance/" + device_id) {
      Serial.println((char)payload[0]);
      int rValue = (char)payload[0] - 48;
      if (rValue != general::maintenance_mode.getValue()) {
        if ((rValue == 1 || rValue == 3) && general::maintenance_mode.getValue() == 0) {
          general::maintenance_mode.setValue(rValue, false);
          debug(IMPORTANT, SETUP, "/////////////////////////////////");
          debug(IMPORTANT, SETUP, "Maintenance Mode activated");
          debug(IMPORTANT, SETUP, "/////////////////////////////////");
          maintenanceMode(rValue);
        }
        if (rValue == 0 && general::maintenance_mode.getValue() == 1) {
          general::maintenance_mode.setValue(rValue, false);
        }
      }
    }
  }

void maintenanceMode(int variant) {
  general::maintenance_mode.setValue(1, false);
  while (general::maintenance_mode.getValue() == 1) {
    Serial.println("Maintenance Mode");
    reconnect();

    delay(5000);
    client.loop();
  }
  debug(IMPORTANT, SETUP, "/////////////////////////////////");
  debug(IMPORTANT, SETUP, "Maintenance Mode disabled");
  debug(IMPORTANT, SETUP, "/////////////////////////////////");

  if (variant == 3) { //implemented to restart the arduino remotely by will
    ESP.restart();
  }
}

  void config_request() {   //TODO: optimation
    getUniqueID();
    subToConfigChannel();
    subscribeToMaintenanceCheck();
    debug(IMPORTANT, SETUP, "Requesting config...");
    client.publish("config/request", device_id.c_str());
    delay(100);
    for (short x = 0; x <= 1000; x++) {
      client.loop();
    }
  }

  void subToConfigChannel() {
    String sub_config_get = "config/get/" + device_id + "/#";
    client.subscribe(sub_config_get.c_str());
    debug(INFO, SETUP, "Subscribed to: " + sub_config_get);
  }

  void subscribeToActivityRequest() {
    String sub_topic = "activity/request";
    client.subscribe(sub_topic.c_str());
    debug(INFO, SETUP, "Subscribed to: " + sub_topic);
  }

  void subscribeToMaintenanceCheck() {
    String sub_topic = "maintenance/" + device_id;
    client.subscribe(sub_topic.c_str());
    Serial.println("Subscribed to: " + sub_topic);
  }

  void config_update(String column, String value) {
    String config_update = column + " = '" +  value + "' WHERE `device_overview`.`device_id` = " + device_id;
    client.publish("config/update", config_update.c_str());
  }
  void config_update(String column, int value) {
    config_update(column, (String)value);
  }

  void mysql_insert(String grade, int co2, double temp, double humidity, double pressure, double altitude) {
    if (!(grade[0] == 'a' && grade[1] == 'u' && grade[2] == 't')) {
      String output = "VALUES ('" + grade + "', " + co2 + ", " + temp + ", " + humidity + ", " + pressure + ", " + altitude + ")";
      client.publish("mysql/insert", output.c_str());
      debug(INFO, DATABASE, "INSERTED into LOG grade: " + grade + " co2: " + co2 + " temp: " + temp + " humidity: " + humidity + " pressure: " + pressure + " altitude: " + altitude);
      //Serial.println("INSERTED into LOG grade: " + grade + " co2: " + co2 + " temp: " + temp + " humidity: " + humidity + " pressure: " + pressure + " altitude: " + altitude);
    }
}


  void getUniqueID() {
    device_id = "";
    //for loop provided by the librarie to get a Unique ID of an arduino board
    for (size_t i = 0; i < UniqueIDsize; i++) {
      if (UniqueID[i] < 0x10)
        //Serial.print("0");
        device_id = device_id + "0";
      device_id = device_id + "" + UniqueID[i];
    }
  }

  void setup_wifi() {
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      timeout++;
      if(timeout > 10)
        break;
    }
    randomSeed(micros());
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }

  void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");
      // Create a random client ID
      String clientId = "ESP8266Client-";
      clientId += String(random(0xffff), HEX);
      // Attempt to connect
      if (client.connect(clientId.c_str())) {
        Serial.println("connected");
        client.subscribe("msg");

      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
      }
    }
  }
