#include "btn.h"


BUTTON_STATE button = {};

uint8_t btn1 = LOW;
uint8_t btn1_prev = LOW;
uint8_t btn2 = LOW;
uint8_t btn2_prev = LOW;

void btn_init(void)
{
    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);
}

void btn_read(void)
{
    btn1 = digitalRead(BUTTON1_PIN);
    btn2 = digitalRead(BUTTON2_PIN);
}

BUTTON_STATE get_button_state()
{
    static int btn1_prev = HIGH;
    static unsigned long pressedTime = 0;
    
    int btn1 = digitalRead(BUTTON1_PIN); // Read button state
    unsigned long currentMillis = millis();

    if (btn1 == LOW && btn1_prev == HIGH) // Button just pressed
    {
        pressedTime = currentMillis;
        btn1_prev = LOW;
    }
    else if (btn1 == HIGH && btn1_prev == LOW) // Button just released
    {
        btn1_prev = HIGH;
        if (currentMillis - pressedTime >= LONG_PRESS_TIME)
        {
            return BUTTON_STATE::LONG_PRESS; // Long press detected
        }
        else
        {
            return BUTTON_STATE::SHORT_PRESS; // Short press detected
        }
    }

    return BUTTON_STATE::OFF; // No press detected
}

bool button_pressed(void)
{
    static unsigned long lastPressTime = 0;
    btn1 = digitalRead(BUTTON1_PIN);
    if (btn1 == LOW && btn1_prev == HIGH)
    {
        if (millis() - lastPressTime > DEBOUNCE_DELAY) // Debounce check
        {
            lastPressTime = millis();
            btn1_prev = LOW;
            return true;
        }
    }
    btn1_prev = btn1;
    return false;
}


