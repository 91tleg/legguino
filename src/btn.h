#ifndef BTN_H
#define BTN_H

#include <Arduino.h>
#include <stdint.h>
#include "state_menu.h"
#include "legguino.h"

#define BUTTON1_PIN 22
#define BUTTON2_PIN 24
#define SHORT_PRESS_TIME 500
#define LONG_PRESS_TIME 1000
#define DEBOUNCE_DELAY 50

enum class BUTTON_STATE : uint8_t
{
	OFF,
	SHORT_PRESS,
	LONG_PRESS
};

void btn_init(void);
void btn_read(void);
BUTTON_STATE get_button_state(void);
bool button_pressed(void);

extern uint8_t btn1;
extern uint8_t btn1_prev;
extern uint8_t btn2;
extern uint8_t btn2_prev;
extern BUTTON_STATE button;

#endif // BTN_H