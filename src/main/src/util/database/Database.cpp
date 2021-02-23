/*
 Database.cpp - Database-Library for Librarys.
*/

#include "Database.h"
#include "../Util.h"

  //global variables
  WiFiClient espClient;
  PubSubClient client(espClient);

  String device_id;
  String device_grade;
  unsigned long lastMsg = 0;
  char msg[MSG_BUFFER_SIZE];
  int value = 0;

  //Database connection
  void setupDatabaseConnection() {
    getUniqueID();
    Serial.println(device_id);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    delay(500);
    //connect client the first time
    if (!client.connected())
      reconnect();
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
      general::state.setValue((short) atoi((char*)payload), false);
      /*String language = "";
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
        general::state.setValue(5, false);*/
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
    debug(NONE, SETUP, "Requesting config...");
    client.publish("config/request", device_id.c_str());
    delay(200);
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
    if (!client.connected())
      reconnect();
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
    int timeout = 0;
    while (!client.connected() && timeout < 2) {
      timeout++;
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
    if(!client.connected())
      Serial.println("Database not connected");
  }
