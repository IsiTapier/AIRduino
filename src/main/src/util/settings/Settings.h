/*
  Settings.h - Settings-Library for Airduino.
*/

#ifndef Settings_h
#define Settings_h

#include "Arduino.h"
#include "../../classes/Setting/Setting.h"

namespace general {
  extern Setting empty;

  extern Setting version;
  extern Setting darkMode;
  extern Setting piezo;
  extern Setting blink;
  extern Setting data;
  extern Setting calibrateTouch;

  extern Setting state;
  extern Setting kernMode;
  extern Setting graphSpeed;
  extern Setting segments;
  extern Setting blinkThickness;
  extern Setting ventilatingTimeout;

  extern Setting debug;
  extern Setting debugSetup;
  extern Setting debugSensor;
  extern Setting debugDisplay;
  extern Setting debugMenu;
  extern Setting debugTouch;
}

namespace colorModes {
  extern Setting variousColors;
  extern Setting coloredChart;
  extern Setting coloredBar;
  extern Setting coloredState;
  extern Setting coloredTime;
  extern Setting coloredValue;

  extern Setting coloredSlider;
}
/*
namespace colors {
  extern Setting* graphColor;
  extern Setting* valueNormal;
  extern Setting* valueRisk;
  extern Setting* valueWarning;
  extern Setting* valueAlarm;
  extern Setting* barColor;

  extern Setting* barSectionColor;
  extern Setting* DataboxColor;
  extern Setting* defaultTextColor;
  extern Setting* ChartBorderColor;
  extern Setting* SliderDotColorOn;
  extern Setting* SliderDotColorOn;
  extern Setting* SliderColorStrip;
  extern Setting* SliderBackgroundColor;
}*/

#endif
