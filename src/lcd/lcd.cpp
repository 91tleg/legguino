#include "lcd.h"

extern LiquidCrystal lcd;
uint8_t lcd_current_page = 0;
uint8_t button1_state = LOW;  // NEXT
uint8_t button2_state = LOW;  // PREV

void lcd_init(void)
{
    pinMode(BUTTON1_PIN, INPUT);
    pinMode(BUTTON2_PIN, INPUT);
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
        lcd.print("set up done");
    #endif
    
}
