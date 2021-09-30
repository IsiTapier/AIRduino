/*
  Database.h - Database-Library for Airduino.
*/

#ifndef Database_h
#define Database_h

#include "Arduino.h"
#include <WiFi.h>
#include "../../extern/PubSubClient/src/PubSubClient.h"
#include "../../extern/ArduinoUniqueID/src/ArduinoUniqueID.h"

#define MSG_BUFFER_SIZE (250)

  //global variables
  extern WiFiClient espClient;
  extern PubSubClient client;

  extern String device_id;
  extern String device_class;
  extern unsigned long lastMsg;
  extern char msg[MSG_BUFFER_SIZE];
  extern int value;
  extern String currentTime;
  extern String currentDate;

  extern const char* ssid;
  extern const char* password;
  extern const char* mqtt_server;

  //Database connection
  void setupDatabaseConnection();
  void callback(char* topic_char, byte* payload, unsigned int length);
  void config_request();
  void maintenanceMode();
  void config_update(String column, String value);
  void config_update(String column, int value);
  void mysql_insert(String grade, int co2, double temp, int decibel);
  void getUniqueID();
  void setup_wifi();
  void setup_mqtt();
  void reconnect();
  void reconnectToMQTT();
  void reconnectToWifi();
  void reconnectSystem();

  void generalSubscriptions();
  void subscribeToMQTT(String topic);
  void subscribeToMQTT(String topic, String c);
  void subscribeToMQTT(String topic, int id);
  void saveClassList(String list);
  String shortenName(String input);

  void reportBug(String title);
  void sendDebugToMQTT(String message);

#endif
