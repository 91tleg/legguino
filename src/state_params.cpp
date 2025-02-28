#include "state_params.h"
#include "btn.h"

ecu_params ecu_parameters = {0};
PARAMS param_current = PARAMS::PARAMS_ONE;

void params_page_control(void)
{
    if (param_current > PARAMS::PARAMS_EIGHT)
        param_current = PARAMS::PARAMS_ONE;
    btn1 = digitalRead(BUTTON1_PIN);
    if (btn1 == LOW && btn1_prev == HIGH)
    {
        PORTB |= (1 << PB7);
        _delay_ms(100);
        PORTB &= ~(1 << PB7);
        param_current = static_cast<PARAMS>(static_cast<uint8_t>(param_current) + 1);
    }
    btn1_prev = digitalRead(BUTTON1_PIN);
}

void read_sensor_one(void)
{
    read_battery_voltage();
    read_speed();
}

void read_sensor_two(void)
{
    read_rpm();
    read_coolant_temp();
}

void read_sensor_three(void)
{
    read_fuel_trim();
    read_airflow();
}

void read_sensor_four(void)
{
    read_atmosphere_pressure();
    read_manifold_pressure();
}

void read_sensor_five(void)
{
    read_injector_pulse_width();
    read_iacv_duty_cycle();
}

void read_sensor_six(void)
{
    read_load();
    read_throttle_percentage();
}

void read_sensor_seven(void)
{
    read_o2_signal();
    read_timing_correction();
}

void read_sensor_eight(void)
{
    read_boost_control_duty_cycle();
    read_throttle_signal();
}

void lcd_print_params_one(void)
{
    lcd.setCursor(0, 0);
    lcd.print("VBAT:");
    lcd.print(ecu_parameters.vb, 1);
    lcd.print("      ");
    lcd.setCursor(0, 1);
    sprintf(buffer, "VSPD:%dmph", ecu_parameters.vsp);
    lcd.print(buffer);
}

void lcd_print_params_two(void)
{
    lcd.setCursor(0, 0);
    sprintf(buffer, "REV:%d   ", ecu_parameters.erev);
    lcd.print(buffer);
    lcd.setCursor(0, 1);
    sprintf(buffer, "WATR:%df  ", ecu_parameters.tw);
    lcd.print(buffer);
}
void lcd_print_params_three(void)
{
    lcd.setCursor(0, 0);
    lcd.print("FUEL TRIM:");
    lcd.print(ecu_parameters.alphar, 1);
    lcd.setCursor(0, 1);
    lcd.print("MAF:");
    lcd.print(ecu_parameters.qa, 1);
    lcd.print("   ");
}

void lcd_print_params_four(void)
{
    lcd.setCursor(0, 0);
    lcd.print("BARO:");
    lcd.print(ecu_parameters.barop, 1);
    lcd.print("torr    ");
    lcd.setCursor(0, 1);
    lcd.print("BOOST:");
    lcd.print(ecu_parameters.manip, 1);
    if (ecu_parameters.manip < 0)
        lcd.print("inHG  ");
    else
        lcd.print("psi   ");
}

void lcd_print_params_five(void)
{
    lcd.setCursor(0, 0);
    lcd.print("INJPW:");
    lcd.print(ecu_parameters.tim, 2);
    lcd.print("   ");
    lcd.setCursor(0, 1);
    sprintf(buffer, "ISC:%d%%      ", ecu_parameters.isc);
    lcd.print(buffer);
}

void lcd_print_params_six(void)
{
    lcd.setCursor(0, 0);
    sprintf(buffer, "LOAD:%d    ", ecu_parameters.ldata);
    lcd.print(buffer);
    lcd.setCursor(0, 1);
    sprintf(buffer, "TPS:%d%%  ", ecu_parameters.tps);
    lcd.print(buffer);
}

void lcd_print_params_seven(void)
{
    lcd.setCursor(0, 0);
    lcd.print("OXY:");
    lcd.print(ecu_parameters.o2r, 1);
    lcd.setCursor(0, 1);
    sprintf(buffer, "TIMING:%d%%", ecu_parameters.advs);
    lcd.print(buffer);
}

void lcd_print_params_eight(void)
{
    lcd.setCursor(0, 0);
    lcd.print("WGC:");
    lcd.print(ecu_parameters.wgc, 1);
    lcd.setCursor(0, 1);
    lcd.print("THV:");
    lcd.print(ecu_parameters.thv, 1);
    lcd.print("  ");
}