#ifndef STATE_MENU_H
#define STATE_MENU_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "state_diag.h"
#include "state_control.h"
#include "state_params.h"
#include "state_race.h"
#include "btn.h"

void display_menu(void);
void select_mode(void);
void check_return_menu(void);

#endif // STAE_MENU_H