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
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    //connect client the first time
    do{
      if (!client.connected()) {
        reconnect();
      }
    } while ((!client.connected()) && requestDecision("MQTT fehlgeschlagen", "erneut versuchen?", "Ja", "Nein"));
    // do{
    config_request();
    // delay(100);
    // } while((!configReceived) && requestDecision("Config nicht geladen", "erneut versuchen?", "Ja", "Nein"));
    
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
          //Serial.println((char)payload[digit]);
          digit++;
          delay(1);
        }
        digit++;
      }
      switch (x) {
        case 0: { //grade
          device_class = output;
          /* Serial.print("Grade ");
          Serial.println(device_class); */
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
    if(general::mode.getValue() > 2) {
      debug(IMPORTANT, SETUP, "/////////////////////////////////");
      debug(IMPORTANT, SETUP, "Maintenance Mode activated");
      debug(IMPORTANT, SETUP, "/////////////////////////////////");
      maintenanceMode();  
    }
  }

void maintenanceMode() {
  byte m = general::mode.getValue();

  display.fillScreen(BACKGROUND_COLOR);
  dPrint(general::maintenance_mode.getTitle(), 160, 120, 3, TEXT_COLOR, 4);

  while (general::mode.getValue() > 2) {
    reconnect();

    delay(500);
    client.loop();
  }
  debug(IMPORTANT, SETUP, "/////////////////////////////////");
  debug(IMPORTANT, SETUP, "Maintenance Mode disabled");
  debug(IMPORTANT, SETUP, "/////////////////////////////////");

  general::mode.setValue(1);
  if (m == 4) { //implemented to restart the arduino remotely by will
    Serial.println("Restart");
    ESP.restart();   
  }
}

  void config_request() {   //TODO: optimation
    getUniqueID();
    subToConfigChannel();
    subscribeToMaintenanceCheck();
    debug(IMPORTANT, SETUP, "Requesting config...");
    client.publish("config/request", device_id.c_str());
    delay(500);
    for (short x = 0; x <= 1000; x++) {
      client.loop();
    }
  }

  void subToConfigChannel() {
    String sub_config_get = "config/get/" + device_id;
    client.subscribe(sub_config_get.c_str());
    debug(INFO, SETUP, "Subscribed to: " + sub_config_get);
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

  void mysql_insert(String grade, int co2, double temp) {
    if (!(grade[0] == 'a' && grade[1] == 'u' && grade[2] == 't')) {
      String output = "VALUES ('" + grade + "', " + co2 + ", " + temp + ")";
      client.publish("mysql/insert", output.c_str());
      debug(INFO, DATABASE, "INSERTED into LOG grade: " + grade + " co2: " + co2 + " temp: " + temp);
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
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    do{
      display.pushImage(0, 0, DISPLAY_LENGTH, DISPLAY_HEIGHT, logoBlatt);
      // We start by connecting to a WiFi networ

      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
    
      for (int x = 0; x <= 15; x++) {
        delay(500);
        Serial.print(".");
      }
    } while ((WiFi.status() != WL_CONNECTED) && requestDecision("Wifi fehlgeschlagen", "erneut versuchen?", "Ja", "Nein"));

    display.fillScreen(BACKGROUND_COLOR);
    if (WiFi.status() == WL_CONNECTED) {
      randomSeed(micros());
      Serial.println("");
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
      String clientId = "ESP8266Client-";
      clientId += String(random(0xffff), HEX);
      // Attempt to connect
      if (client.connect(clientId.c_str())) {
        Serial.println("connected");
        client.subscribe("msg");
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 1 seconds");
        // Wait 1 seconds before retrying
        //delay(10);
      }
    }
    if(!client.connected())
      Serial.println("Database not connected");
  }
