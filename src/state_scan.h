#ifndef STATE_SCAN_H
#define STATE_SCAN_H

#include <Arduino.h>
#include "state_diag.h"
#include "legguino.h"

extern io_switches status0;

enum class SCAN : uint8_t 
{
  /* 00 */ SENSOR_ONE,
  /* 01 */ SENSOR_TWO,
  /* 02 */ SENSOR_THREE,
  /* 03 */ SENSOR_FOUR,
  /* 04 */ INPUT_SW,
  /* 05 */ IO_SW,
  /* 06 */ ACTIVE_TC_ONE,
  /* 07 */ ACTIVE_TC_TWO,
  /* 08 */ ACTIVE_TC_THREE,
  /* 09 */ STORED_TC_ONE,
  /* 10 */ STORED_TC_TWO,
  /* 11 */ STORED_TC_THREE,
  /* 12 */ CLEAR_TC
};

#endif // STATE_SCAN_H