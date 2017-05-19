#include "types.h"

const char * orientation_to_str(orientation_t orientation){
  switch (orientation){
    case UPRIGHT:
      return "upright";
    case INVERTED:
      return "inverted";
    default:
      return "unknown";
  }
}
