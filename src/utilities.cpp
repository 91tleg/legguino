#include "utilities.h"

char buffer[32];
LiquidCrystal_I2C lcd(0x27, 16, 2);
LcdBarGraph_I2C lbg(&lcd, 0, 1);

void print_received_data(uint8_t answer[3])
{
    for (uint8_t i = 0; i < 3; ++i)
    {
        if (answer[i] < 0x10)
        {
            lcd.print(0); // Print leading zero for single-digit hex values
        }
        lcd.print(answer[i], HEX);
        if (i < 2)
        {
            lcd.write(static_cast<uint8_t>('.'));
        }
    }
}

void trigger_feedback_one(void)
{
    PORTB |= (1 << PB7);
    _delay_ms(10);
    PORTB &= ~(1 << PB7);
}

void trigger_feedback_two(void)
{
    PORTB |= (1 << PB7);
    _delay_ms(50);
    PORTB &= ~(1 << PB7);
}

void delay_millis(unsigned long duration) {
    unsigned long start_time = millis();
    while (millis() - start_time < duration) 
    {}
}
