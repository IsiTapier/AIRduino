/*
 Database.cpp - Database-Library for Librarys.
*/

#include "Database.h"
#include "../Util.h"
#include "../../guis/weatherGui/WeatherGui.h"

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
 

//Makros to get client output condition for certain features
#define GET_TOPIC_CLASS(_topic) (topic == _topic + device_class) 
#define GET_TOPIC_ROOM(_topic) (topic == _topic + device_room) 
#define GET_TOPIC_ID(_topic) (topic == _topic + sensorID)
#define GET_TOPIC_CLASS_ROOM(_topic) GET_TOPIC_CLASS(_topic) || GET_TOPIC_ROOM(_topic) 
#define GET_TOPIC_CLASS_ROOM_ID(_topic) GET_TOPIC_CLASS_ROOM(_topic) || GET_TOPIC_ID(_topic) 
#define GET_TOPIC_W_SENSOR_ID "config/get/" + (String) sensorID

  short sensorID = -1; //Wenn auf -1 gesetzt, wird wird ein Wert aus dem EEPROM geholt

  //global variables
  WiFiClient espClient;
  PubSubClient client(espClient);
  AsyncWebServer server(80);

  //Wifi Connection
  const char* WifiSsid[2] = {"JCBS-Schüler", "FRITZ!Box 7590 JG"};
  const char* WifiKeys[2] = {"K1,14DWwFuwuu.", "4400834912335401"};
  byte wifiConnectionDigit = 0;

  //config values
  String device_id;
  String device_class = "";
  String device_room;
  boolean initialConfigReception = false;
  
  //MQTT variables
  unsigned long lastMsg = 0;
  char msg[MSG_BUFFER_SIZE];
  int value = 0;
  int lastGui = GUI_MENU;
  String clientId = "ESP32Client-";

  //Server Addons
  String currentTime = "00:00";
  String currentDate = "01.01.";
  String classPreNames[30];
  String classSecondNames[30];
  String classFinalNames[30];
  int classLength = 0;
  
 
  //Database connection
  void setupDatabaseConnection() {
    manageSensorID();
    getUniqueID();

    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);

    reconnectSystem();
    
    setupAsyncElegantOTA();
  }

  void setupAsyncElegantOTA() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(200, "text/plain", "ElegantOTA Web Server of ESP32 (Class: " + device_class + "; Room: " + device_room + "; ID: " + device_id);
    });

    AsyncElegantOTA.begin(&server);    // Start ElegantOTA
    server.begin();
    Serial.println("HTTP server started");
    Serial.println(WiFi.localIP());
  }

  void manageSensorID() {
    if(sensorID == -1) {
      sensorID = GET_EEPROM_SENSOR_ID;
      if(sensorID == NULL) {
        sensorID = -1;
      }
    } else {
      SET_EEPROM_SENSOR_ID((short)sensorID);
      Serial.print("EEPROM SENSOR ID: "); Serial.println(GET_EEPROM_SENSOR_ID);
    }
    Serial.print("SensorID: "); Serial.println(sensorID);
  }

  void maintenanceMode() {
    debug(IMPORTANT, SETUP, "/////////////////////////////////");
    debug(IMPORTANT, SETUP, "Maintenance Mode activated");
    debug(IMPORTANT, SETUP, "/////////////////////////////////");
    display.fillScreen(BACKGROUND_COLOR);
    dPrint(general::maintenance_mode.getTitle(), 160, 120, 3, TEXT_COLOR, 4);

    while (general::mode.getValue() >= 3) {
      reconnectToMQTT();
      delay(500);
      client.loop();
    }
    debug(IMPORTANT, SETUP, "/////////////////////////////////");
    debug(IMPORTANT, SETUP, "Maintenance Mode disabled");
    debug(IMPORTANT, SETUP, "/////////////////////////////////");
  }

void config_update(String column, String value) {
  if (client.connected()) {
    // reconnectToMQTT();
    String config_update = column + " = '" +  value + "' WHERE `device_overview`.`device_id` = " + device_id;
    client.publish("config/update", config_update.c_str());
    debug(SPAMM, MENUD, "CONFIG - " + column + " = " + (String) value);
  }
}

void config_update(String column, int value) {
  config_update(column, (String)value);
}

void mysql_insert(String grade, int co2, double temp, int decibel) {
  if(grade == "") grade = sensorID;
  String output = "VALUES ('" + grade + "', " + co2 + ", " + temp + ", " + decibel + ")";
  client.publish("mysql/insert", output.c_str());
  debug(INFO, DATABASE, "INSERTED into LOG grade: " + grade + " co2: " + co2 + " temp: " + temp + " decibel: " + decibel);

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


void callback(char* topic_char, byte* payload, unsigned int length) {
  Serial.print("MQTT: (");
  Serial.print(topic_char);
  Serial.print(") :: ");

  String payload_string = "";
  for (int i = 0; i < length; i++) {
    payload_string = payload_string + "" + (char)payload[i];
  }
  //get Topic as a String
  String topic = "";
  for (int i = 0; i < strlen(topic_char); i++) {
    // Serial.print((char)payload[i]);
    topic = topic + "" + (char)topic_char[i];
  }
  Serial.println(payload_string);
  

  if((topic == GET_TOPIC_W_SENSOR_ID + "/class")) {
    device_class = payload_string; 
  } else if((topic == GET_TOPIC_W_SENSOR_ID + "/room")) {
    device_room = payload_string;  
  } else if((topic == GET_TOPIC_W_SENSOR_ID + "/version")) {
    general::version.setValue((short) atoi(payload_string.c_str())); 
  } else if((GET_TOPIC_W_SENSOR_ID + "/sound")) {
    general::sound.setValue((short) atoi(payload_string.c_str())); 
  } else if((GET_TOPIC_W_SENSOR_ID + "/graph_speed")) {
    general::graph_speed.setValue((short) atoi(payload_string.c_str())); 
  } else if((GET_TOPIC_W_SENSOR_ID + "/coloredChart")) {
    colorModes::c_chart.setValue((short) atoi(payload_string.c_str())); 
  } else if((GET_TOPIC_W_SENSOR_ID + "/segments")) {
    general::segments.setValue((short) atoi(payload_string.c_str())); 
  } else if((GET_TOPIC_W_SENSOR_ID + "/ventilating_timeout")) {
    general::ventilating_timeout.setValue((short) atoi(payload_string.c_str())); 
  } else if((GET_TOPIC_W_SENSOR_ID + "/language")) {
    general::language.setValue((short) atoi(payload_string.c_str())); 
  }


  if(GET_TOPIC_CLASS_ROOM_ID("cali/low/")) {
    Meassure::getSensor().calibrate();
    Serial.println("Forced Min Cali via MQTT");
  }
  if(GET_TOPIC_CLASS_ROOM_ID("cali/high/")) {
    int value = atoi(payload_string.c_str());
    Serial.println(value);
    Meassure::getSensor().zeroSpan(value);
    Serial.println("Forced Max Cali via MQTT");
  }
  if(GET_TOPIC_CLASS_ROOM_ID("cali/touch/")) {
    Serial.println("Forced Touch Calibration via MQTT");
    ts.calibration();   
  }
  if(GET_TOPIC_CLASS_ROOM_ID("manager/restart/")) {
      debug(IMPORTANT, SETUP, "ESP restartet via MQTT");
    ESP.restart();
  }
  if(topic == "manager/restart/" + sensorID) {
      debug(IMPORTANT, SETUP, "ESP restartet via MQTT");
    ESP.restart();
  }
  if(GET_TOPIC_CLASS_ROOM_ID("manager/deepSleep/")) {
    int minutes = atoi(payload_string.c_str());
    
    debug(IMPORTANT, SETUP, "ESP wurde für " + String(minutes) + " Minuten in den Sleepmode versetzt");
    ESP.deepSleep(minutes * 60 * 1000000);
  }
  if(GET_TOPIC_CLASS_ROOM_ID("manager/testPeep/")) {
    ledcAttachPin(PIEZO, 0);
    for(int x = 0; x <= 10; x++) {
      if(x%2 == 0) {
        ledcWriteNote(0, NOTE_C, 3);
      } else {
        ledcDetachPin(PIEZO);
      }
      delay(500);
    }
  }
  if(GET_TOPIC_CLASS_ROOM_ID("manager/message/")) {
    display.fillScreen(BLACK);
    dPrint("Admin-Message", 160, 20, 4, LIGHT_BLUE, 4);
    String output;
    int textHight = 45;
    for(int x = 0; x < payload_string.length(); x++) {
        output = output + payload_string[x] + "";
        if((x%15 == 0)) {
          dPrint(output, 20, textHight, 3, GREY);
          output = "";
          textHight += 25;
        }
    }
    general::mode.setValue(LOADINGSCREEN);
    general::mode.setValue(general::mode.getOldValue());
  }
  if(GET_TOPIC_CLASS_ROOM_ID("manager/setGui/")) {
    gui.setValue(atoi(payload_string.c_str()));
    Serial.println("changed gui remotely to " + atoi(payload_string.c_str()));
  }
  //Weather system
  if(topic == "weather/weather") {
    WeatherGui::updateWeather(payload_string);
  }
  if(topic == "weather/windspeed") {
    WeatherGui::updateWindspeed(payload_string);
  }
  if(topic == "weather/temp") {
    WeatherGui::updateTemp(payload_string);
  }
  if(topic == "weather/humidity") {
    WeatherGui::updateHumidity(payload_string);
  }
  if(topic == "weather/forecastWeather3") {
    WeatherGui::updateForecastWeather3(payload_string);
  }
  if(topic == "weather/forecastWeatherTomorrow") {
    WeatherGui::updateForecastWeatherTomorrow(payload_string);
  }
  if(topic == "time") {
    currentTime = payload_string;
    ClockGui::loop();         
  }
  if(topic == "date") {
    currentDate = payload_string;
    ClockGui::loop();          
  }
  if(GET_TOPIC_CLASS_ROOM_ID("class/list/")) {
    saveClassList(payload_string);    
  }
}

void generalSubscriptions() {
  Serial.print("DATABASE: "); Serial.println("General Subscriptions");
  subscribeToMQTT("maintenance/", true);
  subscribeToMQTT("cali/low/", true);
  subscribeToMQTT("cali/high/", true);
  subscribeToMQTT("cali/touch/", true);
  subscribeToMQTT("manager/restart/", true);
  subscribeToMQTT("manager/deepSleep/", true);
  subscribeToMQTT("manager/testPeep/", true);
  subscribeToMQTT("manager/message/", true);
  subscribeToMQTT("class/list/", true);
  subscribeToMQTT("manager/setGui/", true);
  subscribeToMQTT("time");
  subscribeToMQTT("date");
  
  subscribeToMQTT("weather/#");
  subscribeToMQTT("config/get/" + (String) sensorID + "/#");
}

void reconnectSystem() {
  if(!WiFi.isConnected()) { 
     Serial.println("Wifi connect");  
      WiFi.disconnect();
      WiFi.mode(WIFI_STA);
      WiFi.begin(WifiSsid[wifiConnectionDigit%sizeOf(WifiSsid)], WifiKeys[wifiConnectionDigit%sizeOf(WifiKeys)]);
      wifiConnectionDigit++;
      
      WiFi.reconnect();
      vTaskDelay(10000/portTICK_PERIOD_MS);
      Serial.println("Wifi connect");
  }
  if(!client.connected() && WiFi.isConnected()) {
    reconnectToMQTT();
  }
  if(random(1,20) == 20) Serial.println(client.state());
}


void reconnectToMQTT() {
  if(!WiFi.isConnected()) {
    return;
  }
  if(!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    clientId = device_id;
    /* clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX); */
  
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      generalSubscriptions();
      client.publish("config2/request", ((String)sensorID).c_str());
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      vTaskDelay(5000/portTICK_PERIOD_MS);

    }
  }
}

void subscribeToMQTT(String topic) {
  client.subscribe(topic.c_str());
  // debug(IMPORTANT, SETUP, "Subscribed to: " + topic);
}

void subscribeToMQTT(String topic, String c) {
  topic = topic + c;
  client.subscribe(topic.c_str());
  // debug(IMPORTANT, SETUP, "Subscribed to: " + topic);
}

void subscribeToMQTT(String topic, int id) {
  topic = topic + id;
  client.subscribe(topic.c_str());
  // debug(IMPORTANT, SETUP, "Subscribed to: " + topic);
}

void subscribeToMQTT(String topic, boolean everything) {
  if(everything) {
    String tTopic = topic; 
    tTopic += device_class; client.subscribe(tTopic.c_str()); tTopic = topic; 
    tTopic += device_id; client.subscribe(tTopic.c_str()); tTopic = topic; 
    tTopic += device_room; client.subscribe(tTopic.c_str()); tTopic = topic; 
    
    // debug(IMPORTANT, SETUP, "Subscribed to liberally: " + topic);
  }
}

void reportBug(String title) {
  if(requestDecision(title, "wirklich melden?")) {
    String payload = device_class + "," + title;
    client.publish("report", payload.c_str());
  }
}

void sendDebugToMQTT(String message) {

  String adress = "console/" + device_class;
  client.publish(adress.c_str(), message.c_str());
}

void saveClassList(String rawList) {
  String namePile;
  int nameCounter = 0;

  for(int x = 0;  x < rawList.length(); x++) {
    if(rawList[x] == ',') {
      addPreAndLastName(namePile, nameCounter);
      namePile = "";
      nameCounter++;
    } else {
      namePile += rawList[x];
      // Serial.print(rawList[x]);
    }
    if(x <= rawList.length()) classLength = nameCounter;
  }

  shortenAllNames();
  for(int x = 0; x < classLength; x++) { 
    Serial.print(classFinalNames[x]); 
    Serial.print(", ");
  }
  Serial.println();
}

void addPreAndLastName(String fullName, int digit) {
  String preName = "";
  String secondName = "";
  String namePile;
  for(int x = 0; fullName[x] != ' '; x++) {
      preName += fullName[x];
  }
  for(int x = fullName.length()-1; fullName[x] != ' '; x--) {
      secondName += fullName[x];
  }
  
  classPreNames[digit] = preName;
  classSecondNames[digit] = secondName;
}

void shortenAllNames() {
  for(int x = 0; x < classLength; x++) {
    boolean similar = false;
    for(int y = 0; y < classLength; y++) {
      if(x != y) {
        if(classPreNames[x] == classPreNames[y]) {         
          similar = true;
        }
      }
    }
    if(similar) {
      classFinalNames[x] = classPreNames[x] + " " + classSecondNames[x][classSecondNames[x].length() - 1] + "."; 
    } else {
      classFinalNames[x] = classPreNames[x];
    }
  }
}