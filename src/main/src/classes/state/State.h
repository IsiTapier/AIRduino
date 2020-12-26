#include "../../util/Colors.h"
#include "../../util/Util.h"

enum States {
  VENTILATING = -1,
  GOOD = 0,
  MEDIUM = 1,
  BAD = 2,
  BLINK = 3,
  PIEP = 4
};

#define AE char(142)
#define ae char(132)
#define OE char(153)
#define oe char(148)
#define UE char(154)
#define ue String(char(129))
#define SS char(225)


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
    if(_state == VENTILATING)
      return("L"+ue+"ftet");
    if(_state == GOOD)
      return("bon");
    if(_state == MEDIUM)
      return("mal");
    if(_state == BAD)
      return("gammlig");
    if(_state == BLINK)
      return("sehr gammlig");
    if(_state == PIEP)
      return(ue+"ber gammlig");
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
      default: return (PPM_COLOR_A);
        break;
    }
  }

private:
  States _state;
};
