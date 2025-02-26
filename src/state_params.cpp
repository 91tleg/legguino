#include "state_params.h"

ecu_params ecu_parameters = {0};
PARAMS param_current = PARAMS::PARAMS_ONE;

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