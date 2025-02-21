#include "state_menu.h"

MENU menu_current = MENU::SCAN_MODE;

void display_menu(MENU menu_current, LiquidCrystal_I2C *lcd)
{
    switch (menu_current)
    {
    case MENU::SCAN_MODE:
      lcd->setCursor(6, 1);
      lcd->print("SCAN");
      break;
    case MENU::PARAMS_MODE:
      lcd->setCursor(6, 1);
      lcd->print("PARM");
      break;
    case MENU::DIAG_MODE:
      lcd->setCursor(6, 1);
      lcd->print("DIAG");
      break;
    case MENU::RACE_MODE:
      lcd->setCursor(6, 1);
      lcd->print("RACE");
      break;
    case MENU::COMPASS_MODE:
      lcd->setCursor(6, 1);
      lcd->print("NULL");
      break;
    }
}