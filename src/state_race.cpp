#include "state_race.h"

void race_page_control(void)
{
    unsigned long previousMillis = 0;
    unsigned long interval = 100;
    unsigned long currentMillis = millis(); // Get current time
    if (currentMillis - previousMillis >= interval)
    {
        if (race_curr_page > RACE::MP)
        race_curr_page = RACE::TW;
        btn1_read();
        if (btn1 == LOW && btn1_prev == HIGH)
        {
            PORTB |= (1 << PB7);
            _delay_ms(50);
            PORTB &= ~(1 << PB7);
            race_curr_page = static_cast<RACE>(static_cast<uint8_t>(race_curr_page) + 1);
        }
        btn1_prev = btn1;
    }
}

void lcd_print_tw(void)
{
    lcd.setCursor(0, 0);
    sprintf(buffer, "Temp W:%df      ", ecu_parameters.tw);
    lcd.print(buffer);
    lbg.drawValue(ecu_parameters.tw, -47, 401);
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