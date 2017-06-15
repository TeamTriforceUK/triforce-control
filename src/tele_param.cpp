/**
* @file tele_param.cpp
* @author Cameron A. Craig
* @date 14 Jun 2017
* @copyright 2017 Cameron A. Craig
* @brief <brief>
*/

#include "tele_param.h"

const char *unit_to_str(tele_command_unit_t unit) {
  switch(unit) {
    case CU_RPM:
      return "rpm";
    case CU_RPS:
      return "rps";
    case CU_MPSPS:
      return "m/s/s";
    case CU_CELCIUS:
      return "C";
    case CU_VOLTS:
      return "V";
    case CU_DEGREES:
      return "deg";
    case CU_NONE:
      return "";
  }
}
