/*
 * http://www.4bc.org/vanagon/SSM_params.html
 * http://www.alcyone.org.uk/ssm/ecureverse.html
 *
 * https://github.com/P1kachu/ssm1-gc8
 *
 */

#ifndef SSM1_H
#define SSM1_H

#include <stdint.h>
#include <avr/pgmspace.h>
#include <LiquidCrystal_I2C.h>
#include "utilities.h"
#include "addresses.h"

void stop_read(void);
void get_romid(uint8_t *buffer);
/*
 * The ROM ID is a 6 digit hexadecimal number. You can find the ROM ID by sending the "Get RomID" 00 46 48 49
 * when the ECU is transmitting. 46 48 49 is "FHI" (Fuji Heavy Industries) in ASCII representation. The Select
 * Monitor seems to read the ROM ID directly from the ROM at address 8C3D (Alcylone) rather than using the "Get RomID" command.
 * Digit  Meaning
 * 1	    Unknown. Always 7. Does this mean "Subaru"?
 * 2	    Model Year of Car	0=1990, 2=1992, 4=1994 etc. Sometimes this is older than the year the car was actually sold and registered.
 * 3, 4	 Type of car/engine	25=SVX/EG33, 31=Legacy/EJ20, 32=Legacy/EJ22, 36=Legacy/EJ20T, 3F=Impreza/EJ15, 40=Impreza/EJ20T
 * 5	    Domestic Market	1=Japan,2=USA,3=Europe,A=UK
 * 6	    ECU Firmware Revision	Relative to the rest of the ROM ID
 */

uint8_t read_data_from_address(uint16_t addr);
/* 
 * There is no flow control, so you must read the data as fast as the ECU is sending it, otherwise your
 * serial port input buffer will overflow and your program will get out of sync with the data. With this
 * in mind, it makes sense to sanity check the returned msb and lsb bytes, before reporting the data byte.
 */

void read_battery_voltage(void); // x*0.08
void read_speed(void);           // x/1.6
void read_rpm(void);             // x*25
void read_coolant_temp(void);    // Lookup table
void read_airflow(void);         // x/50
void read_throttle_percentage(void);
void read_throttle_signal(void);
void read_manifold_pressure(void);        // x/0.128-1060
void read_boost_control_duty_cycle(void); // x/2.56
void read_ignition_timing(void);          // x   advs
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
void send_clear_command(uint16_t addr);
/*
 * AA MSB LSB 00   (Clear to Zero)
 * AA MSB LSB FF   (If inverted value is used)
 * In order to write data, you must be receiving data. ie. To write FF to address 1234 in the ECU, you would first
 * issue an ECU read command 78123400 and then a write command AA1234FF.
 */

#endif // SSM1_H
