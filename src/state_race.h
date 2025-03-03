#ifndef STATE_RACE_H
#define STATE_RACE_H

#include <Arduino.h>

enum class RACE : uint8_t
{
  /* 00 */ TW,
  /* 01 */ TPS,
  /* 02 */ MP,
};

void race_page_control(void);
void lcd_print_tw(void);
void lcd_print_tps(void);
void lcd_print_mp(void);

extern RACE race_current;

#endif // STATE_RACE