#include "states.h"

const char * state_to_str(state_t state){
  switch(state){
    case STATE_DISARMED:
      return "DISARMED";
    case STATE_DRIVE_ONLY:
      return "DRIVE ONLY";
    case STATE_FULLY_ARMED:
      return "FULLY ARMED";
  }
}
