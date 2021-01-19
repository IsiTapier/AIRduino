/*
  Settings.cpp - Settings-Library for Librarys.
*/

#include "Arduino.h"
#include "Settings.h"

namespace general {
  Setting empty = Setting(EMPTY);
  Setting version = Setting(SWITCH, "Version", 0, false, "'Version' changed", "Version 1", "Version 2");
  //Setting darkMode = Setting(SELECT, "Theme", 0, true, "'Theme' changed", "light mode", "dark mode");
  Setting piezo = Setting(SWITCH, "Piepser", 1, true, "'Pieser' changed"); //TODO
  Setting blink = Setting(SWITCH, "Blink", 1, true, "'Blink' changed");
  //Setting status = Setting(MULTISELECT, "Blink", 0, false, "'Status' changed");
  //Setting calibrateTouch = Setting(ACTIVATOR, "Touch kalibrieren", 1, true, "Touch changed");
  Setting graphSpeed = Setting(EMPTY); //TODO
  Setting kernMode = Setting(SWITCH, "Kern Modus", 0, false, "'Kern Modus' changed");
  Setting segments = Setting(EMPTY); //TODO
  Setting blinkThickness = Setting(EMPTY); //TODO
  Setting ventilatingTimeout = Setting(EMPTY); //TODO
  Setting debugSensor = Setting(SWITCH, "Debug Sensor", 0, true, "'Debug Sensor' changed");
}

namespace colorModes {
  Setting variousColors = Setting(SWITCH, "farbiges Design", 0, true, "'farbiges Disign' changed");
  Setting coloredGraph = Setting(SWITCH, "farbiger Graph", 0, true, "'farbiger Graph' changed");
  Setting coloredBar = Setting(SWITCH, "farbige Leiste", 1, true, "'farbige Leiste' changed");
  Setting coloredState = Setting(SWITCH, "farbiger Status", 1, true, "'farbiger Status' changed");
  Setting coloredTime = Setting(SWITCH, "farbige Zeit", 0, true, "'farbige Zeit' changed");
  Setting coloredValue = Setting(SWITCH, "farbiger Wert", 1, true, "'farbiger Wert' changed");
  Setting coloredSlider = Setting(SWITCH, "farbiger Toggle", 0, true, "'farbiger Toggle' changed");
}

/*namespace colors {
  Setting graphColor = Setting(COLOR "Graphfarbe", 0, true, "'Graphfarbe' changed");
  Setting valueNormal = Setting(COLOR, "Farbe 'Gut'", 0, true, "'' changed");
  Setting valueRisk = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting valueWarning = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting valueAlarm = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting barColor = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");

  Setting barSectionColor = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting DataboxColor = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting defaultTextColor = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting ChartBorderColor = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting SliderDotColorOn = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting SliderDotColorOn = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");

  Setting SliderColorStrip = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting SliderBackgroundColor = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
} */
