#include "state_params.h"

PARAMS param_current = PARAMS::PARAMS_ONE;
extern ecu_params ecu_parameters;

void display_romid(LiquidCrystal_I2C *lcd)
{
    for (uint8_t i = 0; i < 3; ++i)
    {
        if (ecu_parameters.romid_param[i] < 0x10)
        {
            lcd->print(0); // Print leading zero for single-digit hex values
        }
        lcd->print(ecu_parameters.romid_param[i], HEX);
        if (i < 2)
        {
            lcd->write(static_cast<uint8_t>('.'));
        }
    }
}

void read_sensor_one(void)
{
    read_battery_voltage();
    read_speed();
    read_rpm();
    read_coolant_temp();
}

void read_sensor_two(void)
{
    read_fuel_trim();
    read_airflow();
    read_load();
    read_throttle_percentage();
}

void read_sensor_three(void)
{
    read_injector_pulse_width();
    read_iacv_duty_cycle();
    read_o2_signal();
    read_timing_correction();
}

void read_sensor_four(void)
{
    read_atmosphere_pressure();
    read_manifold_pressure();
    read_boost_control_duty_cycle();
    read_throttle_signal();
}