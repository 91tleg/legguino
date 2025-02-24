/*
 * http://www.4bc.org/vanagon/SSM_params.html
 * http://www.alcyone.org.uk/ssm/ecureverse.html
 */

#ifndef SSM1_H
#define SSM1_H

#include <stdint.h>
#include <avr/pgmspace.h>
#include <LiquidCrystal_I2C.h>
#include "utilities.h"
#include "addresses.h"

void stop_read(void);
void display_romid(void);
void query_romid(void);
bool get_romid(uint8_t *buffer);
uint8_t read_data_from_address_ex(short addr, bool read_once_only);
uint8_t read_data_from_address(short addr);
void read_battery_voltage(void); // x*0.08
void read_speed(void);           // x/1.6
void read_rpm(void);             // x*25
void read_coolant_temp(void);    // Lookup table
void read_airflow(void);         // x/50
void read_throttle_percentage(void);
void read_throttle_signal(void);
void read_manifold_pressure(void);        // x/0.128-1060
void read_boost_control_duty_cycle(void); // x/2.56
void read_ignition_timing(void);          // x
void read_load(void);                     // x
void read_injector_pulse_width(void);     // x*0.128
void read_iacv_duty_cycle(void);          // x/2
void read_o2_signal(void);                // x/100
void read_timing_correction(void);        // x
void read_fuel_trim(void);                // (x-128)/1.28
void read_atmosphere_pressure(void);      // x*1.25+500
void read_input_switches(void);
void read_io_switches(void);
void read_trouble_code_one(uint16_t addr);
void read_active_trouble_code_one(void);
void read_stored_trouble_code_one(void);
void read_trouble_code_two(uint16_t addr);
void read_active_trouble_code_two(void);
void read_stored_trouble_code_two(void);
void read_trouble_code_three(uint16_t addr);
void read_active_trouble_code_three(void);
void read_stored_trouble_code_three(void);
void send_clear_command(uint16_t addr_high, uint16_t addr_low);

#endif // SSM1_H
