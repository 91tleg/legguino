#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "definitions.h"
#include "dbg.h"

enum class MENU : uint8_t
{
  /* 00 */ SCAN_MODE,
  /* 01 */ PARAMS_MODE,
  /* 02 */ DIAG_MODE,
  /* 03 */ RACE_MODE,
  /* 04 */ PARAMS_BIG,
  /* 05 */ COMPASS_MODE
};

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

enum class PARAMS : uint8_t
{
  /* 00 */ PARAMS_ONE,
  /* 01 */ PARAMS_TWO,
  /* 02 */ PARAMS_THREE,
  /* 03 */ PARAMS_FOUR,
  /* 04 */ PARAMS_FIVE,
  /* 05 */ PARAMS_SIX,
  /* 06 */ PARAMS_SEV,
  /* 07 */ PARAMS_EIGHT
};

enum class DIAG : uint8_t
{
  /* 00 */ IN,
  /* 01 */ IO,
  /* 02 */ AC_ONE,
  /* 03 */ AC_TWO,
  /* 04 */ AC_THREE,
  /* 05 */ SC_ONE,
  /* 06 */ SC_TWO,
  /* 07 */ SC_THREE,
  /* 08 */ CLEAR
};

enum class RACE : uint8_t
{
  /* 00 */ TW,
  /* 01 */ TPS,
  /* 02 */ MP,
  /* 03 */ RPM,
  /* 04 */ VSP
};

enum class BIG : uint8_t
{
  /* 00 */ BIG_ONE,
  /* 01 */ BIG_TWO,
  /* 02 */ BIG_THREE,
  /* 03 */ BIG_FOUR,
  /* 04 */ BIG_FIVE,
  /* 05 */ BIG_SIX,
  /* 06 */ BIG_SEV,
  /* 07 */ BIG_EIGHT
};

#if defined LCD
void init(void);
#endif

/*
 * menu
 *
 * +scan mode
 * |
 * +---all params
 * 
 * 
 * +race mode
 * |
 * +---coolant temp
 * |
 * +---throttle percentage
 * |
 * +---manifold pressure
 * |
 * +---engine speed
 * |
 * +---vsp
 * 
 * +compass mode
 * |
 * +---baro
 *
 */

#endif