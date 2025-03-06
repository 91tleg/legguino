#include "legguino.h"
#include "state_menu.h"
#include "btn.h"

MENU menu_current = MENU::SCAN_MODE;

void display_menu()
{
	switch (menu_current)
	{
	case MENU::SCAN_MODE:
		lcd.setCursor(0, 1);
		lcd.print("scan mode");
		break;
	case MENU::PARAMS_MODE:
		lcd.setCursor(0, 1);
		lcd.print("param mode");
		break;
	case MENU::DIAG_MODE:
		lcd.setCursor(0, 1);
		lcd.print("diag mode ");
		break;
	case MENU::RACE_MODE:
		lcd.setCursor(0, 1);
		lcd.print("race mode");
		break;
	}
}

void select_mode(void)
{
	lcd.setCursor(0, 0);
	lcd.print("Select mode:    ");

	unsigned long lastBtn1Press = 0;
    unsigned long lastBtn2Press = 0;
	while (true)
	{
		btn_read();
		display_menu();

		unsigned long currentMillis = millis();
		if (btn1 == LOW && btn1_prev == HIGH && (currentMillis - lastBtn1Press > 200))
		{
			lastBtn1Press = currentMillis;
			PORTB |= (1 << PB7);
			_delay_ms(20);
			PORTB &= ~(1 << PB7);
			menu_current = static_cast<MENU>(static_cast<uint8_t>(menu_current) + 1);
			if (menu_current > MENU::RACE_MODE)
				menu_current = MENU::SCAN_MODE;
		}
		btn1_prev = btn1;

		if (btn2 == LOW && btn2_prev == HIGH && (currentMillis - lastBtn2Press > 200))
		{
			lastBtn2Press = currentMillis;
			PORTB |= (1 << PB7);
			_delay_ms(10);
			PORTB &= ~(1 << PB7);
			lcd.clear();
			return;
		}
		btn2_prev = btn2;
	}
}

void check_return_menu()
{
    static unsigned long pressStartTime = 0;
    static bool buttonHeld = false;
    const unsigned long longPressTime = 1000;

    btn2 = digitalRead(BUTTON2_PIN);
    unsigned long currentMillis = millis();

    // Button just pressed
    if (btn2 == LOW && btn2_prev == HIGH) {
        pressStartTime = currentMillis;
        buttonHeld = false; 
    }

    // Button still held down
    if (btn2 == LOW && !buttonHeld) {
        if (currentMillis - pressStartTime >= longPressTime) {
            buttonHeld = true;
			PORTB |= (1 << PB7);
			_delay_ms(15);
			PORTB &= ~(1 << PB7);
            lcd.clear();
            select_mode(); // Return to menu
        }
    }
    btn2_prev = btn2;
}
