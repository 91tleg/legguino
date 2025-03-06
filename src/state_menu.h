#ifndef STATE_MENU_H
#define STATE_MENU_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

enum class MENU : uint8_t
{
  /* 00 */ SCAN_MODE,
  /* 01 */ PARAMS_MODE,
  /* 02 */ DIAG_MODE,
  /* 03 */ RACE_MODE,
};

void display_menu(void);
void select_mode(void);
void check_return_menu(void);

extern MENU menu_current;

#endif // STAE_MENU_H