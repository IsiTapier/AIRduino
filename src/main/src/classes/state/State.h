/*
  State.h - State-Library for Airduino.
*/

#ifndef State_h
#define State_h

#include "../../util/Colors.h"
#include "../../util/settings/Settings.h"

enum States {
  VENTILATING = -1,
  GOOD = 0,
  MEDIUM = 1,
  BAD = 2,
  BLINK = 3,
  PIEP = 4
};

#define AE String(char(142))
#define ae String(char(132))
#define OE String(char(153))
#define oe String(char(148))
#define UE String(char(154))
#define ue String(char(129))
#define SS String(char(223))
#define ag String(char(224))
#define aa String(char(225))
#define ac String(char(226))
#define eg String(char(232))
#define ea String(char(233))
#define ec String(char(234))
#define oa String(char(243))


extern String stateTitles[6][5];


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
