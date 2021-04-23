/*
 Database.cpp - Database-Library for Librarys.
*/

#include "Database.h"
#include "../Util.h"

  //global variables
  WiFiClient espClient;
  PubSubClient client(espClient);

  String device_id;
  String device_class;
  unsigned long lastMsg = 0;
  char msg[MSG_BUFFER_SIZE];
  int value = 0;

  boolean configReceived = false;

  //Database connection
  void setupDatabaseConnection() {
    getUniqueID();
    Serial.println(device_id);
    setup_wifi();
    if(WiFi.status() == WL_CONNECTED) {
      client.setServer(mqtt_server, 1883);
      client.setCallback(callback);
      delay(500);
      if(!client.connected())
        reconnect();

      while ((!client.connected()) && requestDecision("MQTT fehlgeschlagen", "erneut versuchen?", "Ja", "Nein")) {
        drawLogo();
        for (int x = 0; (x <= 5) && (!client.connected()); x++) {
          delay(500);
          Serial.print(".");
          reconnect();
        }
      }
      
      if(client.connected()) {
        config_request();
        unsigned long timeout = millis();
        do{
          if(millis()-timeout >= 1000)
            timeout = millis();
          drawLogo();
          client.loop();
        } while(!configReceived && (millis()-timeout < 1000 || requestDecision("Config nicht geladen", "erneut versuchen?", "Ja", "Nein")));

        subscribeToMQTT("cali/low/", device_class);
        subscribeToMQTT("cali/high/", device_class);
        subscribeToMQTT("cali/touch/", device_class);
      }
    }
  }

  void callback(char* topic_char, byte* payload, unsigned int length) {
    Serial.print("MQTT: (");
    Serial.print(topic_char);
    Serial.print(") :: ");

    //Write payload into console; put payload into var
    String payload_string = "";
    for (int i = 0; i < length; i++) {
      payload_string = payload_string + "" + (char)payload[i];
    }
    Serial.println(payload_string);


    //get Topic as a String
    String topic = "";
    for (int i = 0; i < strlen(topic_char); i++) {
      //Serial.print((char)payload[i]);
      topic = topic + "" + (char)topic_char[i];
    }

    //get config via a single String
    if (topic == "config/get/" + device_id) {
      debug(IMPORTANT, SETUP, "Config received");
      configReceived = true;

      int digit = 0;      
      for (int x = 0; digit < length; x++) { //loop for every setting
        String output = "";
        if (digit <= length) {
          for (int d = 0; (d < 100) && ((char)payload[digit] != ',') && ((char)payload[digit] != ';'); d++) { //loop to loop the single digits
            output = output + "" + (char)payload[digit];
            digit++;
            delay(1);
          }
          digit++;
        }

        switch (x) {
          case 0: {
            device_class = output;
            if (device_class[0] == 'a' && device_class[1] == 'u' && device_class[2] == 't') { //if the grade is auto generated
              debug(ERROR, DATABASE, "///////////////////// CONFIG ///////////////////////////");
              debug(ERROR, DATABASE, "Please enter the grade of your device into the database");
              debug(ERROR, DATABASE, "////////////////////////////////////////////////////////");
            }
          }
            break;
          case 1: general::version.setValue((short) atoi(output.c_str()), false);
            break;
          case 2: general::mode.setValue((short) atoi(output.c_str()), false);
            break;
          case 3: general::theme.setValue((short) atoi(output.c_str()), false);
            break;
          case 4: general::language.setValue((short) atoi(output.c_str()), false);
            break;
          case 5: general::sound.setValue((short) atoi(output.c_str()), false);
            break;
          case 6: general::blink.setValue((short) atoi(output.c_str()), false);
            break;
          case 7: general::graph_speed.setValue((short) atoi(output.c_str()), false);
            break;
          case 8: general::segments.setValue((short) atoi(output.c_str()), false);
            break;
          case 9: general::blink_thickness.setValue((short) atoi(output.c_str()), false);
            break;
          case 10: general::ventilating_timeout.setValue((short) atoi(output.c_str()), false);
            break;
          case 11: colorModes::c_design.setValue((short) atoi(output.c_str()), false);
            break;
          case 12: colorModes::c_chart.setValue((short) atoi(output.c_str()), false);
            break;
          case 13: colorModes::c_bar.setValue((short) atoi(output.c_str()), false);
            break;
          case 14: colorModes::c_state.setValue((short) atoi(output.c_str()), false);
            break;
          case 15: colorModes::c_time.setValue((short) atoi(output.c_str()), false);
            break;
          case 16: colorModes::c_value.setValue((short) atoi(output.c_str()), false);
            break;
          case 17: colorModes::c_slider.setValue((short) atoi(output.c_str()), false);
            break;
        }
      }
    }

    if(topic == "cali/low/" + device_class) {
      Meassure::getSensor().calibrate();
      Serial.println("Forced Min Cali via MQTT");
    }
    if(topic == "cali/high/" + device_class) {
      int value = atoi(payload_string.c_str());
      Serial.println(value);
      Meassure::getSensor().zeroSpan(value);
      Serial.println("Forced Max Cali via MQTT");
    }
    if(topic == "cali/touch/" + device_class) {
      Serial.println("Forced Touch Calibration via MQTT");
      ts.calibration();   
    }
  }

  void maintenanceMode() {
    debug(IMPORTANT, SETUP, "/////////////////////////////////");
    debug(IMPORTANT, SETUP, "Maintenance Mode activated");
    debug(IMPORTANT, SETUP, "/////////////////////////////////");
    display.fillScreen(BACKGROUND_COLOR);
    dPrint(general::maintenance_mode.getTitle(), 160, 120, 3, TEXT_COLOR, 4);

    while (general::mode.getValue() >= 3) {
      reconnect();
      delay(500);
      client.loop();
    }
    debug(IMPORTANT, SETUP, "/////////////////////////////////");
    debug(IMPORTANT, SETUP, "Maintenance Mode disabled");
    debug(IMPORTANT, SETUP, "/////////////////////////////////");
  }

  void config_request() {   //TODO: optimation
    getUniqueID();
    subscribeToMQTT("config/get/", device_id);
    subscribeToMQTT("maintenance/", device_id);

    debug(IMPORTANT, SETUP, "Requesting config...");
    client.publish("config/request", device_id.c_str());
    delay(500);
    client.loop();
  }

  void config_update(String column, String value) {
    if (client.connected()) {
      reconnect();
      String config_update = column + " = '" +  value + "' WHERE `device_overview`.`device_id` = " + device_id;
      client.publish("config/update", config_update.c_str());
      debug(SPAMM, MENUD, "CONFIG - " + column + " = " + (String) value);
    }
  }

  void config_update(String column, int value) {
    config_update(column, (String)value);
  }

  void mysql_insert(String grade, int co2, double temp) {
    if (!(grade[0] == 'a' && grade[1] == 'u' && grade[2] == 't') && client.connected()) {
      String output = "VALUES ('" + grade + "', " + co2 + ", " + temp + ")";
      client.publish("mysql/insert", output.c_str());
      debug(INFO, DATABASE, "INSERTED into LOG grade: " + grade + " co2: " + co2 + " temp: " + temp);
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
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    // We start by connecting to a WiFi networ
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    delay(500);
    for(int z = 0; (z <= 2) && (WiFi.status() != WL_CONNECTED); z++) {
      WiFi.begin(ssid, password);
      for (int x = 0; (x <= 5) && (WiFi.status() != WL_CONNECTED); x++) {
        delay(400);
        Serial.print(".");
      }
    }
    while ((WiFi.status() != WL_CONNECTED) && requestDecision("Wifi fehlgeschlagen", "erneut versuchen?", "Ja", "Nein")) {
      drawLogo();
      for(int z = 0; (z <= 2) && (WiFi.status() != WL_CONNECTED); z++) {
        WiFi.begin(ssid, password);
        for (int x = 0; (x <= 5) && (WiFi.status() != WL_CONNECTED); x++) {
          delay(500);
          Serial.print(".");
        }
      }
    }
    Serial.println();
    if (WiFi.status() == WL_CONNECTED) {
      randomSeed(micros());
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    }
  }

  void reconnect() {
    // Loop until we're reconnected
    int timeout = 0;
    while (!client.connected() && timeout < 1) {
      timeout++;
      Serial.print("Attempting MQTT connection...");
      // Create a random client ID
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);
      // Attempt to connect
      if (client.connect(clientId.c_str())) {
        Serial.println("connected");
        client.subscribe("msg");
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 1 seconds");
      }
    }
    if(!client.connected())
      Serial.println("Database not connected");
  }

  void subscribeToMQTT(String topic) {
    client.subscribe(topic.c_str());
    debug(INFO, SETUP, "Subscribed to: " + topic);
  }

  void subscribeToMQTT(String topic, String c) {
    topic = topic + c;
    client.subscribe(topic.c_str());
    debug(INFO, SETUP, "Subscribed to: " + topic);
  }

  void subscribeToMQTT(String topic, int id) {
    topic = topic + id;
    client.subscribe(topic.c_str());
    debug(INFO, SETUP, "Subscribed to: " + topic);
  }

  void reportBug(String title) {
    if(requestDecision(title, "wirklich melden?")) {
      String payload = device_class + "," + title;
      client.publish("report", payload.c_str());
    }
  }
