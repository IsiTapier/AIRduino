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
  {"ventilating", "we", "okay", "bad", "horrible"},
  {"a"+ea+"rer", "bon", "agr"+ea+"able", "dang"+ea+"reux", "terrible"},
  {"ventilasi", "Nggih", "ditrima", "ala", "bejat"},
  {"ventilacion", "bien", "agradable", "malo", "catastr"+oa+"fico"},
  {"es ist nicht kalt", "l"+oe+"tig", "bon", "zut", "d"+ea+"gueulasse"}
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
    return stateTitles[general::state.getValue()][_state+1];
  }

  int getColor(boolean colored) {
    if(!colored)
      return(DEFAULT_COLOR);
    switch ((int) _state) {
      case -1:  return (CYAN);
        break;
      case 0: return (PPM_COLOR_N);
        break;
      case 1: return (PPM_COLOR_R);
        break;
      case 2: return (PPM_COLOR_A);
        break;
      case 3: return (PPM_COLOR_A);
        break;
      case 4: return (PPM_COLOR_A);
        break;
      default: return (PPM_COLOR_A);
        break;
    }
  }

private:
  States _state;

};

#endif
