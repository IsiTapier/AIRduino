/*
  Settings.cpp - Settings-Library for Librarys.
*/

#include "Arduino.h"
#include "Settings.h"

namespace general {
  Setting empty = Setting(EMPTY);
  
  Setting version = Setting(SWITCH, "version", {"Version"}, 0, "'Version' changed", 0, 1, {{"Version 1"}, {"Version 2"}}, false);
  Setting theme = Setting(SELECT, "theme", {"Theme"}, 1, "'Theme' changed", 0, 1, {{"light mode"}, {"dark mode"}});
  Setting piezo = Setting(SWITCH, "peep", {"Piepser"}, 1, "'Pieser' changed");
  Setting blink = Setting(SWITCH, "blink", {"Blink"}, 1, "'Blink' changed");
  Setting data = Setting(SWITCH, "", {"Data"}, 1, "Data gettering changed", 0, 1, {{"generierte Daten", "generated data", "data g"+ea+"nerat"+ea}, {"Sensor Daten", "sensor data", "data de sensor"}}); //TODO: connect to database
  Setting calibrateTouch = Setting(ACTIVATOR, "", {"Touch kalibrieren"});

  Setting language = Setting(MULTISELECT, "state", {"Status"}, 0, "'Status' changed", 0, 5, {{"deutsch"}, {"englisch"}, {"franz"+oe+"sisch"}, {"java"}, {"spanisch"}, {"special"}});
  Setting kernMode = Setting(SWITCH, "", {"Kern Modus"}, 0, "'Kern Modus' changed");
  Setting graphSpeed = Setting(NUMBER, "graphSpeed", {"Graph Dauer"}, 5, "'Graph Speed' changed", 1, 9);
  Setting segments = Setting(NUMBER, "menu_segments", {"Menu Felder"}, 6, "'Segments' changed", 1, 6);
  Setting blinkThickness = Setting(NUMBER, "blinkThickness", {"Blink Dicke"}, 2, "'Blink Boarder Thickness' changed", 1, 9);
  Setting ventilatingTimeout = Setting(NUMBER, "ventilatingTimeout", {"Timeout"}, 5, "'ventilatingTimeout' changed", 1, 9);

  Setting debugPriority = Setting(MULTISELECT, "", {"Debug"}, 0, "'Debug' changed", 0, 6, {{"None"}, {"Error"}, {"Warning"}, {"Important"}, {"Debug"}, {"Info"}, {"Spamm"}});
  Setting debugSetup = Setting(SWITCH, "", {"Debug Setup"}, 0, "'Debug Setup' changed");
  Setting debugSensor = Setting(SWITCH, "", {"Debug Sensor"}, 0, "'Debug Sensor' changed");
  Setting debugDisplay = Setting(SWITCH, "", {"Debug Display"}, 0, "'Debug Display' changed");
  Setting debugMenu = Setting(SWITCH, "", {"Debug Menu"}, 0, "'Debug Menu' changed");
  Setting debugTouch = Setting(SWITCH, "", {"Debug Touch"}, 0, "'Debug Touch' changed");

  Setting maintenance_mode = Setting(MULTISELECT, "maintenance_mode", {"Wartungsmodus"}, 0, "'Wartungsmodus' geändert", 0, 3);

}

namespace colorModes {
  Setting variousColors = Setting(SWITCH, "c_design", {"farbiges Design"}, 0, "'farbiges Disign' changed");
  Setting coloredChart = Setting(SWITCH, "c_chart", {"farbiger Graph"}, 1, "'farbiger Graph' changed");
  Setting coloredBar = Setting(SWITCH, "c_bar", {"farbige Leiste"}, 1, "'farbige Leiste' changed");
  Setting coloredState = Setting(SWITCH, "c_state", {"farbiger Status"}, 0, "'farbiger Status' changed");
  Setting coloredTime = Setting(SWITCH, "c_time", {"farbige Zeit"}, 1, "'farbige Zeit' changed");
  Setting coloredValue = Setting(SWITCH, "c_value", {"farbiger Wert"}, 1, "'farbiger Wert' changed");

  Setting coloredSlider = Setting(SWITCH, "c_slider", {"farbiger Toggle"}, 0, "'farbiger Toggle' changed");
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
