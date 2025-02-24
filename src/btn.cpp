#include "btn.h"

uint8_t btn1 = LOW;
uint8_t btn1_prev = LOW;
uint8_t btn2 = LOW;
uint8_t btn2_prev = LOW;

void btn_init(void)
{
    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);
}

void btn_read(void)
{
    btn1 = digitalRead(BUTTON1_PIN);
    btn2 = digitalRead(BUTTON2_PIN);
}