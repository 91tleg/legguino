#include "state_race.h"
#include "btn.h"
#include "ssm1.h"
#include "legguino.h"
#include "state_params.h"

RACE race_current = RACE::TW;

void race_page_control(void)
{
    if (race_current > RACE::MP)
        race_current = RACE::TW;
    btn1 = digitalRead(BUTTON1_PIN);
    if (btn1 == LOW && btn1_prev == HIGH)
    {
        PORTB |= (1 << PB7);
        _delay_ms(500);
        PORTB &= ~(1 << PB7);
        race_current = static_cast<RACE>(static_cast<uint8_t>(race_current) + 1);
    }
    btn1_prev = digitalRead(BUTTON1_PIN);
}

void lcd_print_tw(void)
{
    lcd.setCursor(0, 0);
    sprintf(buffer, "Temp W:%df     ", ecu_parameters.tw);
    lcd.print(buffer);
    lbg.drawValue(ecu_parameters.tw, -50, 275);
}

void lcd_print_tps(void)
{
    lcd.setCursor(0, 0);
    sprintf(buffer, "Throttle:%d%%    ", ecu_parameters.tps);
    lcd.print(buffer);
    lbg.drawValue(ecu_parameters.tps, 100);
}

void lcd_print_mp(void)
{
    lcd.setCursor(0, 0);
    lcd.print("Boost:");
    lcd.print(ecu_parameters.manip, 2);
    if (ecu_parameters.manip < 0)
        lcd.print("inHG    ");
    else
        lcd.print("psi    ");
    lbg.drawValue(static_cast<int16_t>(ecu_parameters.manip), -25, 20);
}