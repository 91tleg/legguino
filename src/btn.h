#ifndef BTN_H
#define BTN_H

#include <Arduino.h>
#include <stdint.h>
#include "state_menu.h"
#include "legguino.h"

void btn1_init(void);
void btn2_init(void);
void btn1_read(void);
void btn2_read(void);
bool btn1_pressed(void);
bool btn2_pressed(void);

extern uint8_t btn1;
extern uint8_t btn1_prev;
extern uint8_t btn2;
extern uint8_t btn2_prev;

#endif // BTN_H