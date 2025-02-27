#include "state_race.h"
#include "btn.h"
RACE race_current = RACE::TW;

void race_page_control(void)
{
    if (race_current > RACE::MP)
    race_current = RACE::TW;
    btn1 = digitalRead(BUTTON1_PIN);
    if (btn1 == LOW && btn1_prev == HIGH)
    {
        PORTB |= (1 << PB7);
        _delay_ms(100);
        PORTB &= ~(1 << PB7);
        race_current = static_cast<RACE>(static_cast<uint8_t>(race_current) + 1);
    }
    btn1_prev = digitalRead(BUTTON1_PIN);
}