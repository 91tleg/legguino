#include "lcd.h"

SCAN lcd_current_page = SCAN::SENSOR_ONE;
MENU menu_current = MENU::SCAN_MODE;
PARAMS param_current = PARAMS::PARAMS_ONE;
DIAG diag_current = DIAG::IN;
RACE race_current = RACE::TW;
BIG big_current = BIG::BIG_ONE;

#ifdef LCD
extern LiquidCrystal lcd;


void init(void)
{
    pinMode(A14, OUTPUT);
    pinMode(A13, OUTPUT);
    pinMode(A4, OUTPUT);
    pinMode(A0, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A1, OUTPUT);
    digitalWrite(A14, LOW);
    digitalWrite(A13, HIGH);
    digitalWrite(A4, LOW);
    digitalWrite(A0, LOW);
    digitalWrite(A2, LOW);
    digitalWrite(A1, HIGH);
    lcd.begin(16, 2);

#if PRINT_DEBUG_MESSAGES_ON_LCD
    lcd.print("lcd set up done");
#endif
}

#endif