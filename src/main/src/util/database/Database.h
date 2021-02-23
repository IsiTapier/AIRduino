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
  extern String device_grade;
  extern unsigned long lastMsg;
  extern char msg[MSG_BUFFER_SIZE];
  extern int value;

  extern const char* ssid;
  extern const char* password;
  extern const char* mqtt_server;

  //Database connection
  void setupDatabaseConnection();
  void callback(char* topic_char, byte* payload, unsigned int length);
  void config_request();
  void maintenanceMode(int variant);
  void subToConfigChannel();
  void subscribeToActivityRequest();
  void subscribeToMaintenanceCheck();
  void config_update(String column, String value);
  void config_update(String column, int value);
  void mysql_insert(String grade, int co2, double temp, double humidity, double pressure, double altitude);
  void getUniqueID();
  void setup_wifi();
  void reconnect();

#endif
