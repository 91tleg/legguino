#include <Arduino.h>
#include <stdint.h>
#include "btn.h"
#include "pindef.h"

uint8_t btn1 = LOW;
uint8_t btn1_prev = LOW;
uint8_t btn2 = LOW;
uint8_t btn2_prev = LOW;

void btn_init(void)
{
    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);

     #if PRINT_DEBUG_MESSAGES_ON_LCD
        lcd.print("btn set up done");
    #endif
}

void read_btns(void)
{
    btn1 = digitalRead(BUTTON1_PIN);
    btn1_prev = digitalRead(BUTTON1_PIN);
    btn2 = digitalRead(BUTTON2_PIN);
    btn2_prev = digitalRead(BUTTON2_PIN);
}