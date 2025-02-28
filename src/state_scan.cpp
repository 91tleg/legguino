#include "state_scan.h"
#include "btn.h"
SCAN lcd_current_page = SCAN::SENSOR_ONE;

void scan_page_control(void)
{
    if (lcd_current_page > SCAN::CLEAR_TC)
        lcd_current_page = SCAN::SENSOR_ONE;
    btn1 = digitalRead(BUTTON1_PIN);
    if (btn1 == LOW && btn1_prev == HIGH)
    {
        PORTB |= (1 << PB7);
        _delay_ms(100);
        PORTB &= ~(1 << PB7);
        lcd_current_page = static_cast<SCAN>(static_cast<uint8_t>(lcd_current_page) + 1);
    }
    btn1_prev = digitalRead(BUTTON1_PIN);
}