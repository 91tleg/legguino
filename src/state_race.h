#ifndef STATE_RACE_H
#define STATE_RACE_H

#include <Arduino.h>
#include "parameters.h"
#include "state.h"
#include "utilities.h"
#include "btn.h"

void race_page_control(void);
void lcd_print_tw(void);
void lcd_print_tps(void);
void lcd_print_mp(void);

#endif // STATE_RACE_H