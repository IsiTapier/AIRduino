/*
  State.h - State-Library for Airduino.
*/

#ifndef State_h
#define State_h

#include "../../util/Colors.h"
#include "../../util/settings/Settings.h"
#include "../../display/Design.h"
#include "arduino.h"

enum States {
  VENTILATING = -1,
  GOOD = 0,
  MEDIUM = 1,
  BAD = 2,
  BLINK = 3,
  PIEP = 4
};

const String stateTitles[6][5] = {
  {"L"+ue+"ftet", "Gut", "Ok", "Schlecht", "L"+ue+"ften!"},
  {"ventilating", "well", "okay", "bad", "horrible"},
  {"a"+ea+"rer", "bon", "agr"+ea+"able", "dang"+ea+"reux", "terrible"},
  {"ventilasi", "Nggih", "ditrima", "ala", "bejat"},
  {"ventilacion", "bien", "agradable", "malo", "catastr"+oa+"fico"},
  {"es ist nicht kalt", "l"+oe+"tig", "bon", "zut", "dang it"}
};

class State {
public:
  State() = default;

  constexpr State(States aState) : _state(aState) { }

  constexpr operator int() const { return (int) _state; }
  constexpr explicit operator States() const { return _state; }  // Allow switch and comparisons.
                                              // note: Putting constexpr here causes
                                              // clang to stop warning on incomplete
                                              // case handling.
  constexpr explicit operator bool() = delete;        // Prevent usage: if(fruit)
  constexpr State operator=(const States a) const { return State(a); }
  constexpr State operator=(const int a) const { return State((States) a); }

  String getTitle() {
    return stateTitles[general::language.getValue()][_state+(_state==PIEP?0:1)];
  }

  int getColor(boolean colored = true) {
    if(!colored)
      return(DEFAULT_COLOR);
    switch ((int) _state) {
      case -1:  return (CYAN);
        break;
      case 0: return (COLOR_STATUS_NORMAL);
        break;
      case 1: return (COLOR_STATUS_RISK);
        break;
      case 2: return (COLOR_STATUS_WARNING);
        break;
      case 3: return (COLOR_STATUS_ALARM);
        break;
      case 4: return (COLOR_STATUS_ALARM);
        break;
      default: return (COLOR_STATUS_ALARM);
        break;
    } 
  }
  int getBackgroundColor() {
    switch ((int) _state) {
      case -1:  return (BACKGROUND_COLOR_STATUS_VENTILATING_V3);
        break;
      case 0: return (BACKGROUND_COLOR_STATUS_NORMAL_V3);
        break;
      case 1: return (BACKGROUND_COLOR_STATUS_RISK_V3);
        break;
      case 2: return (BACKGROUND_COLOR_STATUS_ALARM_V3);
        break;
      case 3: return (BACKGROUND_COLOR_STATUS_ALARM_V3);
        break;
      case 4: return (BACKGROUND_COLOR_STATUS_ALARM_V3);
        break;
      default: return (BACKGROUND_COLOR_STATUS_ALARM_V3);
        break;
    } 
  }

private:
  States _state;

};

#endif
