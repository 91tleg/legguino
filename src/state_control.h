#ifndef STATE_SCAN_H
#define STATE_SCAN_H

#include <Arduino.h>
#include "state_diag.h"

enum class SCAN : uint8_t 
{
  /* 00 */ SENSOR_ONE,
  /* 01 */ SENSOR_TWO,
  /* 02 */ SENSOR_THREE,
  /* 03 */ SENSOR_FOUR,
  /* 04 */ SENSOR_FIVE,
  /* 05 */ SENSOR_SIX,
  /* 06 */ SENSOR_SEVEN,
  /* 07 */ SENSOR_EIGHT,
  /* 08 */ INPUT_SW,
  /* 09 */ IO_SW,
  /* 10 */ ACTIVE_TC_ONE,
  /* 11 */ ACTIVE_TC_TWO,
  /* 12 */ ACTIVE_TC_THREE,
  /* 13 */ STORED_TC_ONE,
  /* 14 */ STORED_TC_TWO,
  /* 15 */ STORED_TC_THREE,
  /* 16 */ CLEAR_TC
};

void scan_page_control(void);

extern SCAN lcd_current_page;

#endif // STATE_SCAN_H