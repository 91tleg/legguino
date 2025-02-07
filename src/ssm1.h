#ifndef SSM1_H
#define SSM1_H

#include <stdint.h>
#include <avr/pgmspace.h>
#include "utilities.h"

/*
 * http://www.4bc.org/vanagon/SSM_params.html
 * http://www.alcyone.org.uk/ssm/ecureverse.html
 */

#define EARLY_TURBO_ROMID 0x7136
#define TURBO_ROMID 0x7236
#define JECS1_ROMID 0x7031
#define JECS2_ROMID 0x7232
#define JECS3_ROMID 0x7332

#define BATTERY_VOLTAGE_ADDR 0x1404
#define SPEED_ADDR 0x154B
#define RPM_ADDR 0x140B
#define COOLANT_ADDR 0x1405
#define IGNITION_ADVANCE_ADDR 0x1489
#define AIRFLOW_SENSOR_ADDR 0x1400
#define ENGINE_LOAD_ADDR 0x1414
#define THROTTLE_ADDR 0x1487
#define INJECTOR_PULSE_WIDTH_ADDR 0x15F0
#define ISU_DUTY_VALVE_ADDR 0x158A
#define O2_AVERAGE_ADDR 0x1403 // Current o2 reading (not average)
//#define O2_MINIMUM_ADDR 0x0000
//#define O2_MAXIMUM_ADDR 0x0000
#define KNOCK_CORRECTION_ADDR 0x1530
#define AF_CORRECTION_ADDR 0x1488
#define ATMOSPHERIC_PRESSURE_ADDR 0x1516
#define MANIFOLD_PRESSURE_ADDR 0x00BE
#define BOOST_SOLENOID_ADDR 0x144D
#define INPUT_SWITCHES_ADDR 0x15A8 // [IG.AT.!UD.!RM.xx.!NT.PK.FC]
#define IO_SWITCHES_ADDR 0x15A9 // [ID.AC.AR.RF.FP.CN.KS.xx]
#define ACTIVE_TROUBLE_CODE_ONE_ADDR 0x0047
#define ACTIVE_TROUBLE_CODE_TWO_ADDR 0x0048
#define ACTIVE_TROUBLE_CODE_THREE_ADDR 0x0049
#define STORED_TROUBLE_CODE_ONE_ADDR 0x1604
#define STORED_TROUBLE_CODE_TWO_ADDR 0x1605
#define STORED_TROUBLE_CODE_THREE_ADDR 0x1606

constexpr uint8_t NUM_READ_TRIES = 3;

void stop_read(void);
void query_romid(void);
bool get_romid(byte *buffer);
byte read_data_from_address_ex(short addr, bool read_once_only);
byte read_data_from_address(short addr);
void read_battery_voltage(void); // x*0.08
void read_speed(void);           // x/1.6
void read_rpm(void);             // x*25
void read_coolant_temp(void);    // Lookup table
void read_airflow(void);         // x/50
void read_throttle_percentage(void);
void read_throttle_signal(void);
void read_manifold_pressure(void);         // x/0.128-1060
void read_boost_control_duty_cycle(void);  // x/2.56
void read_ignition_timing(void);           // x
void read_load(void);                      // x
void read_injector_pulse_width(void);      // x*0.128
void read_iacv_duty_cycle(void);           // x/2
void read_o2_signal(void);                 // x/100
void read_timing_correction(void);         // x
void read_fuel_trim(void);                 // (x-128)/1.28
void read_atmosphere_pressure(void);       // x*1.25+500
void read_input_switches(void);
void read_io_switches(void);
void read_active_trouble_code_one(void);
void read_active_trouble_code_two(void);
void read_active_trouble_code_three(void);
void read_stored_trouble_code_one(void);
void read_stored_trouble_code_two(void);
void read_stored_trouble_code_three(void);
void send_clear_command(uint16_t addr_high, uint16_t addr_low);

#endif /* SSM1_H */
