#include "state_handle.h"

void handle_sensor_one(void)
{
    read_sensor_one();
    lcd_print_params_one();
}

void handle_sensor_two(void)
{
    read_sensor_two();
    lcd_print_params_two();
}

void handle_sensor_three(void)
{
    read_sensor_three();
    lcd_print_params_three();
}

void handle_sensor_four(void)
{
    read_sensor_four();
    lcd_print_params_four();
}

void handle_sensor_five(void)
{
    read_sensor_five();
    lcd_print_params_five();
}

void handle_sensor_six(void)
{
    read_sensor_six();
    lcd_print_params_six();
}

void handle_sensor_seven(void)
{
    read_sensor_seven();
    lcd_print_params_seven();
}

void handle_sensor_eight(void)
{
    read_sensor_eight();
    lcd_print_params_eight();
}

void handle_clear(void)
{
    lcd.setCursor(0, 0);
    lcd.print("CLEAR CODES?    ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    check_clear_code();
}

void handle_input_sw(void)
{
    read_input_switches();
    lcd_print_input_sw();
}

void handle_io_sw(void)
{
    read_io_switches();
    lcd_print_io_sw();
}

void handle_active_one(void)
{
    read_active_trouble_code_one();
    lcd_print_code_one();
}

void handle_active_two(void)
{
    read_active_trouble_code_two();
    lcd_print_code_two();
}

void handle_active_three(void)
{
    read_active_trouble_code_three();
    lcd_print_code_three();
}

void handle_stored_one(void)
{
    read_stored_trouble_code_one();
    lcd_print_code_one();
}

void handle_stored_two(void)
{
    read_stored_trouble_code_two();
    lcd_print_code_two();
}

void handle_stored_three(void)
{
    read_stored_trouble_code_three();
    lcd_print_code_three();
}

void handle_race_one(void)
{
    read_coolant_temp();
    lcd_print_tw();
}

void handle_race_two(void)
{
    read_throttle_percentage();
    lcd_print_tps();
}

void handle_race_three(void)
{
    read_manifold_pressure();
    lcd_print_mp();
}