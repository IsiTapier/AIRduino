/*
  Debug.cpp - Debug-Library for Librarys.
*/

#include "Debug.h"

using namespace general;


void debug(DebugPriority priority, DebugType type, String title, double value, String end) {
  if(debugPriority.getValue() >= priority && ((type == SETUP && debugSetup.getValue()) || (type == SENSOR && debugSensor.getValue()) || (type == DISPLAYD && debugDisplay.getValue()) || (type == MENUD && debugMenu.getValue()) || (type == TOUCH && debugTouch.getValue()) || (type == DATABASE && debugDatabase.getValue()))) {
    String message = title + ": " + String(value) + " " + end;
    Serial.println(message);
    sendDebugToMQTT(message);
  }
}

void debug(DebugPriority priority, DebugType type, String title, int value, String end) {
  if(debugPriority.getValue() >= priority && ((type == SETUP && debugSetup.getValue()) || (type == SENSOR && debugSensor.getValue()) || (type == DISPLAYD && debugDisplay.getValue()) || (type == MENUD && debugMenu.getValue()) || (type == TOUCH && debugTouch.getValue()) || (type == DATABASE && debugDatabase.getValue()))) {
    String message = title + ": " + String(value) + " " + end;
    Serial.println(message);
    sendDebugToMQTT(message);
  }
}

void debug(DebugPriority priority, DebugType type, String title, String value, String end) {
  if(debugPriority.getValue() >= priority && ((type == SETUP && debugSetup.getValue()) || (type == SENSOR && debugSensor.getValue()) || (type == DISPLAYD && debugDisplay.getValue()) || (type == MENUD && debugMenu.getValue()) || (type == TOUCH && debugTouch.getValue()) || (type == DATABASE && debugDatabase.getValue()))) {
    String message = title + ": " + value + " " + end;
    Serial.println(message);
    sendDebugToMQTT(message);
  }
}

void debug(DebugPriority priority, DebugType type, String message) {
  if(debugPriority.getValue() >= priority && ((type == SETUP && debugSetup.getValue()) || (type == SENSOR && debugSensor.getValue()) || (type == DISPLAYD && debugDisplay.getValue()) || (type == MENUD && debugMenu.getValue()) || (type == TOUCH && debugTouch.getValue()) || (type == DATABASE && debugDatabase.getValue()))) {
    Serial.println(message);
    sendDebugToMQTT(message);
  }
}

