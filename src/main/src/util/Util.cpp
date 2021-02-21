/*
  Util.cpp - Util-Library for Librarys.
*/

#include "Arduino.h"
#include "Util.h"

  TFT_eSPI display(DISPLAY_HEIGHT, DISPLAY_LENGTH);
  TouchScreen ts(TFT_D1, TFT_DC, TFT_CS, TFT_D0, TOUCH_RESISTANCE);

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

  const char* ssid = "FRITZ!Box 7590 JG";
  const char* password = "4400834912335401";
  const char* mqtt_server = "192.168.178.57";
  /*const char* ssid = "AG-iOT";
    const char* password = "#Wlan4iOT#JCBS-AG!";
    const char* mqtt_server = "192.168.178.57";*/

  void setupDatabaseConnection() {
    getUniqueID();
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    delay(500);
    //connect client the first time
    if (!client.connected()) {
      reconnect();
    }
    //config_request();
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

    if (topic == "config/get/" + String(device_id) + "/grade") {
      device_grade = "";
      for (int i = 0; i < length; i++) {
        device_grade = device_grade + "" + (char)payload[i];
      }
    }

    if (topic == "config/get/" + String(device_id) + "/version") {
      general::version.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + String(device_id) + "/maintenance_mode") {
      //general: = atoi((char*)payload), false);
    }

    if (topic == "config/get/" + String(device_id) + "/theme") {
      general::theme.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + String(device_id) + "/state") {
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

    if (topic == "config/get/" + String(device_id) + "/peep") {
      general::piezo.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + String(device_id) + "/blink") {
      general::blink.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + String(device_id) + "/graphSpeed") {
      general::graphSpeed.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + String(device_id) + "/menu_segments") {
      general::segments.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + String(device_id) + "/blinkThickness") {
      general::blinkThickness.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + String(device_id) + "/ventilatingTimeout") {
      general::ventilatingTimeout.setValue((short) atoi((char*)payload), false); //int x = *payload
    }

    if (topic == "config/get/" + String(device_id) + "/c_design") {
      colorModes::variousColors.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + String(device_id) + "/c_chart") {
      colorModes::coloredChart.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + String(device_id) + "/c_bar") {
      colorModes::coloredBar.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + String(device_id) + "/c_state") {
      colorModes::coloredState.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + String(device_id) + "/c_time") {
      colorModes::coloredTime.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + String(device_id) + "/c_value") {
      colorModes::coloredValue.setValue((short) atoi((char*)payload), false);
    }

    if (topic == "config/get/" + String(device_id) + "/c_slider") {
      colorModes::coloredSlider.setValue((short) atoi((char*)payload), false);
    }

    //check activity

    if ("" + topic == "activity/request") {
      client.publish("activity/check", String(device_id).c_str());
    }
  }

  void config_request() {   //TODO: optimation
    getUniqueID();
    subToConfigChannel();
    Serial.println("Requesting config...");
    snprintf (msg, sizeof(msg), "%s", String(device_id).c_str());
    client.publish("config/request", msg);
    for (short x = 0; x <= 1000; x++) {
      client.loop();
      delay(1);
    }
  }

  void subToConfigChannel() {
    String sub_config_get = "config/get/" + String(device_id) + "/#";
    client.subscribe(sub_config_get.c_str());
    Serial.println("Subscribed to: " + sub_config_get);
  }

  void subscribeToActivityRequest() {
    String sub_topic = "activity/request";
    client.subscribe(sub_topic.c_str());
    Serial.println("Subscribed to: " + sub_topic);
  }

  void config_update(String column, String value) {
    String config_update = "UPDATE `device_overview` SET `" + column + "` = '" + value + "' WHERE `device_overview`.`String(device_id)` = " + String(device_id);
    client.publish("config/update", config_update.c_str());
  }


  void mysql_insert(String grade, int co2, double temp, double humidity, double pressure, double altitude) {
    snprintf (msg, sizeof(msg), "INSERT INTO `device_log`(`grade`, `co2`, `temp`, `humidity`, `pressure`, `altitude`, `time`) VALUES ('%s',%ld,%f,%f,%f,%f,CURRENT_TIMESTAMP())", grade.c_str(), co2, temp, humidity, pressure, altitude);
    client.publish("mysql/insert", msg);
    Serial.println("INSERTED into LOG grade: " + grade + " co2: " + co2 + " temp: " + temp + " humidity: " + humidity + " pressure: " + pressure + " altitude: " + altitude);
  }


  void getUniqueID() {
    String(device_id) = "";
    //for loop provided by the librarie to get a Unique ID of an arduino board
    for (size_t i = 0; i < UniqueIDsize; i++) {
      if (UniqueID[i] < 0x10)
        //Serial.print("0");
        String(device_id) = String(device_id) + "0";
      String(device_id) = String(device_id) + "" + UniqueID[i];
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

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
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
