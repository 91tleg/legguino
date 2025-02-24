#ifndef BTN_H
#define BTN_H

#include <Arduino.h>
#include <stdint.h>
#include "definitions.h"

#define BUTTON1_PIN 22
#define BUTTON2_PIN 24

void btn_init(void);
void btn_read(void);

extern uint8_t btn1;
extern uint8_t btn1_prev;
extern uint8_t btn2;
extern uint8_t btn2_prev;

#endif // BTN_H