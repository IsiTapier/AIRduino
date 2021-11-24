/*
  Settings.h - Settings-Library for Airduino.
*/

#ifndef Settings_h
#define Settings_h

#include "Arduino.h"
#include "../../classes/Setting/Setting.h"
#include "../../display/Design.h"

namespace general {
  extern Setting empty;

  extern Setting version;
  extern Setting theme;
  extern Setting sound;
  extern Setting blink;
  extern Setting data;
  extern Setting calibrateTouch;

  extern Setting language;
  extern Setting kernMode;
  extern Setting graph_speed;
  extern Setting segments;
  extern Setting blink_thickness;
  extern Setting ventilating_timeout;

  extern Setting debugPriority;
  extern Setting debugSetup;
  extern Setting debugSensor;
  extern Setting debugDisplay;
  extern Setting debugMenu;
  extern Setting debugTouch;

  extern Setting debugDatabase;
  extern Setting developperSettings;
  extern Setting autoTimeReset;

  extern Setting mode;
  extern Setting gui;
  extern Setting maintenance_mode;
  extern Setting reportSystem;
  extern Setting manualReconnect;
}

namespace colorModes {
  extern Setting c_design;
  extern Setting c_chart;
  extern Setting c_bar;
  extern Setting c_state;
  extern Setting c_time;
  extern Setting c_value;

  extern Setting c_slider;
  extern Setting showTopBar;
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

namespace developper {
  extern Setting calibrateMin;
  extern Setting calibrateMax;
  extern Setting autoCalibrationMin;
  extern Setting deviceInfo;
  extern Setting screenShot;
  extern Setting isMappingActive;
  extern Setting triggerCalibrateGui;
}

namespace report {
  extern Setting sensorError;
  extern Setting sensorWrong;
  extern Setting graphicalBug;
  extern Setting acousticBug;
  extern Setting helpShortterm;
  extern Setting developerEmail;
}


#endif
