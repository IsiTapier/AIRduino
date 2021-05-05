/*
  Debug.h - Debug-Library for Airduino.
*/

#ifndef Debug_h
#define Debug_h

#include "Arduino.h"
#include "../settings/Settings.h"
#include "../database/Database.h"

enum DebugPriority {
  NONE = 0,
  ERROR = 1,
  WARNING = 2,
  IMPORTANT = 3,
  DEBUG = 4,
  INFO = 5,
  SPAMM = 6
};

enum DebugType {
  SETUP,
  SENSOR,
  DISPLAYD,
  MENUD,
  TOUCH,
  DATABASE
};

void debug(DebugPriority priority, DebugType type, String title, double value, String end = "");
void debug(DebugPriority priority, DebugType type, String title, int value, String end = "");
void debug(DebugPriority priority, DebugType type, String title, String value, String end = "");
void debug(DebugPriority priority, DebugType type, String message);


#endif
