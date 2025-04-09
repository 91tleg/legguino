#include "btn.h"

uint8_t btn1 = LOW;
uint8_t btn1_prev = LOW;
uint8_t btn2 = LOW;
uint8_t btn2_prev = LOW;

void btn1_init(void)
{
    DDRA &= ~(1 << PA0);  // Set pin 22 as input
    PORTA |= (1 << PA0);  // Enable pull-up resistor
}

void btn2_init(void)
{
    DDRA &= ~(1 << PA2);  // Set pin 24 as input
    PORTA |= (1 << PA2);  // Enable pull-up resistor
}

void __attribute__((weak)) btn1_read(void)
{
    btn1 = (PINA & (1 << PA0)) ? HIGH : LOW;
}

void __attribute__((weak)) btn2_read(void)
{
    btn2 = (PINA & (1 << PA2)) ? HIGH : LOW;
}

bool __attribute__((weak)) btn1_pressed(void)
{
    btn1_read();
    if (btn1 == LOW && btn1_prev == HIGH)
    {
        btn1_prev = btn1;
        return true;
    }
    btn1_prev = btn1;
    return false;
}

bool __attribute__((weak)) btn2_pressed(void)
{
    btn2_read();
    if (btn2 == LOW && btn2_prev == HIGH)
    {
        btn2_prev = btn2;
        return true;
    }
    btn2_prev = btn2;
    return false;
}