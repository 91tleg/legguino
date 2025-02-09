#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "pindef.h"
#include "dbg.h"

#define NUM_COLS 16
#define NUM_ROWS 2
#define LCD_MAX_PAGE_COUNT 13

enum class LCD_PAGE : uint8_t {
  /* 00 */ LCD_SENSOR_ONE,
  /* 01 */ LCD_SENSOR_TWO,
  /* 02 */ LCD_SENSOR_THREE,
  /* 03 */ LCD_SENSOR_FOUR,
  /* 04 */ LCD_INPUT_SW,
  /* 05 */ LCD_IO_SW,
  /* 06 */ LCD_ACTIVE_TC_ONE,
  /* 07 */ LCD_ACTIVE_TC_TWO,
  /* 08 */ LCD_ACTIVE_TC_THREE,
  /* 09 */ LCD_STORED_TC_ONE,
  /* 10 */ LCD_STORED_TC_TWO,
  /* 11 */ LCD_STORED_TC_THREE,
  /* 12 */ LCD_CLEAR_TC
};

void lcd_init(void);

#endif