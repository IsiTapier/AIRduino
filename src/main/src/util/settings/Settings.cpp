/*
  Settings.cpp - Settings-Library for Librarys.
*/

#include "Arduino.h"
#include "Settings.h"

namespace general {
  Setting empty = Setting(EMPTY);

  Setting version = Setting(SWITCH, "Version", 0, false, "'Version' changed", {"Version 1", "Version 2"});
  //Setting darkMode = Setting(SELECT, "Theme", 0, true, "'Theme' changed", "light mode", "dark mode");
  Setting piezo = Setting(SWITCH, "Piepser", 1, true, "'Pieser' changed"); //TODO
  Setting blink = Setting(SWITCH, "Blink", 1, true, "'Blink' changed");
  Setting data = Setting(SWITCH, "Data", 1, true, "Data gettering changed", {"generierte Daten", "Sensor Daten"});
  Setting calibrateTouch = Setting(ACTIVATOR, "Touch kalibrieren");

  Setting state = Setting(MULTISELECT, "Status", 0, false, "'Status' changed", {"deutsch", "englisch", "französisch", "java", "spanisch", "special"});
  Setting graphSpeed = Setting(EMPTY); //TODO
  Setting kernMode = Setting(SWITCH, "Kern Modus", 0, false, "'Kern Modus' changed");
  Setting segments = Setting(EMPTY); //TODO
  Setting blinkThickness = Setting(EMPTY); //TODO
  Setting ventilatingTimeout = Setting(EMPTY); //TODO

  Setting debug = Setting(SWITCH, "Debug", 1, true, "'Debug' changed");
  Setting debugSetup = Setting(SWITCH, "Debug Setup", 1, true, "'Debug Setup' changed");
  Setting debugSensor = Setting(SWITCH, "Debug Sensor", 0, true, "'Debug Sensor' changed");
  Setting debugDisplay = Setting(SWITCH, "Debug Display", 0, true, "'Debug Display' changed");
  Setting debugMenu = Setting(SWITCH, "Debug Menu", 0, true, "'Debug Menu' changed");
  Setting debugTouch = Setting(SWITCH, "Debug Touch", 0, true, "'Debug Touch' changed");
}

namespace colorModes {
  Setting variousColors = Setting(SWITCH, "farbiges Design", 0, true, "'farbiges Disign' changed");
  Setting coloredChart = Setting(SWITCH, "farbiger Graph", 1, true, "'farbiger Graph' changed");
  Setting coloredBar = Setting(SWITCH, "farbige Leiste", 1, true, "'farbige Leiste' changed");
  Setting coloredState = Setting(SWITCH, "farbiger Status", 0, true, "'farbiger Status' changed");
  Setting coloredTime = Setting(SWITCH, "farbige Zeit", 1, true, "'farbige Zeit' changed");
  Setting coloredValue = Setting(SWITCH, "farbiger Wert", 1, true, "'farbiger Wert' changed");

  Setting coloredSlider = Setting(SWITCH, "farbiger Toggle", 0, true, "'farbiger Toggle' changed");
}

/*namespace colors {
  Setting* graphColor = Setting(COLOR "Graphfarbe", 0, true, "'Graphfarbe' changed");
  Setting* valueNormal = Setting(COLOR, "Farbe 'Gut'", 0, true, "'' changed");
  Setting* valueRisk = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting* valueWarning = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting* valueAlarm = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting* barColor = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");

  Setting* barSectionColor = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting* DataboxColor = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting* defaultTextColor = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting* ChartBorderColor = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting* SliderDotColorOn = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting* SliderDotColorOn = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");

  Setting* SliderColorStrip = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting* SliderBackgroundColor = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
} */
