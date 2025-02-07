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
// #define SVX92_ROMID 0x7225
// #define SVX94_ROMID 0x7425

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
#define O2_MINIMUM_ADDR 0x0000
#define O2_MAXIMUM_ADDR 0x0000
#define KNOCK_CORRECTION_ADDR 0x1530 //?
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

const byte coolant_look_up_table[256] PROGMEM = 
{
    // The first 14 values need 255 added
    146,132,117,105, 90, 76, 63, 48, 36, 29, 22, 15,  9,  2,
                                                            250,243,
    237,234,230,226,223,219,216,212,208,205,203,199,198,194,192,189,
    187,185,183,181,180,178,176,174,172,171,169,167,165,163,162,160,
    158,156,154,153,153,151,149,147,147,145,144,144,142,140,140,138,
    138,136,135,135,133,131,131,129,129,127,127,126,126,124,124,122,
    122,120,120,118,118,117,117,115,115,113,113,111,111,109,109,108,
    108,106,106,104,104,102,102,100,100, 99, 99, 97, 97, 97, 95, 95,
     95, 93, 93, 91, 91, 90, 90, 88, 88, 86, 86, 84, 84, 84, 82, 82,
     82, 81, 81, 79, 79, 79, 77, 77, 77, 75, 75, 73, 73, 73, 72, 72,
     72, 70, 70, 68, 68, 68, 66, 66, 66, 64, 64, 63, 63, 61, 61, 59,
     59, 57, 57, 55, 55, 55, 54, 54, 54, 52, 52, 50, 50, 50, 48, 48,
     48, 46, 46, 45, 45, 43, 43, 41, 41, 39, 39, 37, 37, 37, 36, 36,
     36, 34, 34, 32, 32, 30, 30, 28, 28, 27, 27, 25, 25, 23, 23, 21,
     21, 19, 18, 18, 16, 14, 14, 12, 12, 10,  9,  9,  7,  5,  5,  3,
      3,  1,  0,
                  2,  4,  6,  8,  9,  9, 11, 13, 15, 17, 18, 20, 22,
     24, 27, 31, 33, 36, 40, 42, 45, 47, 47, 47, 47, 47, 47, 47, 47   
    // The last 29 values need to be inverted (to negative)
};

void stop_read(void);
void query_romid(void);
bool get_romid(byte *buffer);
byte read_data_from_address_ex(short addr, bool read_once_only);
byte read_data_from_address(short addr);
float read_battery_voltage(void); // x*0.08
uint8_t read_speed(void);         // x/1.6
uint16_t read_rpm(void);          // x*25
int16_t read_coolant_temp(void);  // Lookup table
uint16_t read_airflow(void);      // x/50
uint16_t read_throttle_percentage(void);
uint8_t read_throttle_signal(void);
int16_t read_manifold_pressure(void);         // x/0.128-1060
uint16_t read_boost_control_duty_cycle(void); // x/2.56
uint16_t read_ignition_timing(void);          // x
uint16_t read_load(void);                     // x
uint16_t read_injector_pulse_width(void);     // x*0.128
uint16_t read_iacv_duty_cycle(void);          // x/2
uint16_t read_o2_signal(void);                // x/100
uint16_t read_o2_min(void);
uint16_t read_o2_max(void);
uint16_t read_timing_correction(void);   // x
uint16_t read_fuel_trim(void);           // (x-128)/1.28
uint16_t read_atmosphere_pressure(void); // x*1.25+500
void read_input_switches(void);
void read_io_switches(void);
void read_active_trouble_code_one(void);
void read_active_trouble_code_two(void);
void read_active_trouble_code_three(void);
void read_stored_trouble_code_one(void);
void read_stored_trouble_code_two(void);
void read_stored_trouble_code_three(void);
void send_clear_command(uint16_t addr_high, uint16_t addr_low);

/*; 
; turbo ECU
turbo_params dw romid_param
             dw hitachi_vb, hitachi_vsp, hitachi_erev
             dw hitachi_tw, hitachi_advs, hitachi_qa
             dw hitachi_ldata, hitachi_thv, hitachi_tps, hitachi_tim_t
             dw hitachi_isc, hitachi_o2r, hitachi_alphar
             dw hitachi_rtrd, hitachi_wgc, hitachi_barop_t
             dw hitachi_manip, hitachi_fa0_t, hitachi_fa1_t
             dw hitachi_u1, hitachi_u2, hitachi_u3_t
             dw hitachi_m1_t, hitachi_m2_t, hitachi_m3_t
             dw hitachi_clear_t
             dw 0000h

; early turbo ECU -- uses alternate pulse width
early_turbo_params dw romid_param
             dw hitachi_vb, hitachi_vsp, hitachi_erev
             dw hitachi_tw, hitachi_advs, hitachi_qa
             dw hitachi_ldata, hitachi_thv, hitachi_tps, hitachi_tim_n
             dw hitachi_isc, hitachi_o2r, hitachi_alphar
             dw hitachi_rtrd, hitachi_wgc, hitachi_barop_t
             dw hitachi_manip, hitachi_fa0_t, hitachi_fa1_t
             dw hitachi_u1, hitachi_u2, hitachi_u3_t
             dw hitachi_m1_n, hitachi_m2_n, hitachi_m3_et
             dw hitachi_clear_et
             dw 0000h

; Hitachi non-turbo ECU
hitachi_params dw romid_param
               dw hitachi_vb, hitachi_vsp, hitachi_erev
               dw hitachi_tw, hitachi_advs, hitachi_qa
               dw hitachi_ldata, hitachi_thv, hitachi_tps, hitachi_tim_n
               dw hitachi_isc, hitachi_o2r, hitachi_alphar
               dw hitachi_rtrd, hitachi_barop_n, hitachi_fa0_n
               dw hitachi_fa1_n
               dw hitachi_u1, hitachi_u2, hitachi_u3_n
               dw hitachi_m1_n, hitachi_m2_n, hitachi_m3_n
               dw hitachi_clear_n
               dw 0000h

; 1990-1991 JECS non-turbo ECU
jecs1_params dw romid_param
             dw jecs1_vb, jecs1_vsp, jecs1_erev
             dw jecs1_tw, jecs1_advs, jecs1_qa
             dw jecs1_ldata, jecs1_thv
             dw jecs1_tim, jecs1_isc
             dw jecs1_o2r, jecs1_alphar, jecs1_rtrd
             dw jecs1_barop, jecs1_fa0, jecs1_fa1
             dw jecs1_u1, jecs1_u2, jecs1_u3
             dw jecs1_m1, jecs1_m2, jecs1_m3
             dw jecs1_clear
             dw 0000h

; 1992 JECS non-turbo ECU
jecs2_params dw romid_param
             dw jecs2_vb, jecs2_vsp, jecs2_erev
             dw jecs2_tw, jecs2_advs, jecs2_qa
             dw jecs2_ldata, jecs2_thv
             dw jecs2_tim, jecs2_isc
             dw jecs2_o2r, jecs2_alphar, jecs2_rtrd
             dw jecs2_barop, jecs2_fa0, jecs2_fa1
             dw jecs2_u1, jecs2_u2, jecs2_u3
             dw jecs2_m1, jecs2_m2, jecs2_m3
             dw jecs2_clear
             dw 0000h

; 1993-1994 JECS non-turbo ECU -- has no barometric pressure reading
jecs3_params dw romid_param
             dw jecs2_vb, jecs2_vsp, jecs2_erev
             dw jecs2_tw, jecs2_advs, jecs2_qa
             dw jecs2_ldata, jecs2_thv
             dw jecs2_tim, jecs2_isc
             dw jecs2_o2r, jecs2_alphar, jecs2_rtrd
             dw jecs2_fa0, jecs2_fa1
             dw jecs2_u1, jecs2_u2, jecs2_u3
             dw jecs2_m1, jecs2_m2, jecs2_m3
             dw jecs2_clear
             dw 0000h
*/

typedef struct 
{
    uint32_t romid_param;
    uint16_t vb;
    uint16_t vsp;
    uint16_t erev;
    uint16_t tw;
    uint16_t advs;
    uint16_t qa;
    uint16_t ldata;
    uint16_t thv;
    uint16_t tps;
    uint16_t tim;
    uint16_t isc;
    uint16_t o2r;
    uint16_t alphar;
    uint16_t manip;
    uint16_t rtrd;
    uint16_t barop;
} ecu_params;

typedef struct
{
    uint8_t ignition : 1;
    uint8_t auto_trans : 1;
    uint8_t test_mode : 1;
    uint8_t read_mode : 1;
    uint8_t unused_bit : 1;
    uint8_t neutral : 1;
    uint8_t park : 1;
    uint8_t california : 1;
} input_switches;

typedef struct
{
    uint8_t idle_sw : 1;
    uint8_t ac_sw : 1;
    uint8_t ac_relay : 1;
    uint8_t rad_fan : 1;
    uint8_t fuel_pump : 1;
    uint8_t purge_valve : 1;
    uint8_t pinging : 1;
    uint8_t press_exch : 1;
} io_switches;

typedef struct
{
    uint8_t crank : 1;
    uint8_t starter : 1;
    uint8_t cam : 1;
    uint8_t inj_1 : 1;
    uint8_t inj_2 : 1;
    uint8_t inj_3 : 1;
    uint8_t inj_4 : 1;
    uint8_t unused_bit_7 : 1;
} trouble_code_one;

typedef struct
{
    uint8_t temp : 1;
    uint8_t knock : 1;
    uint8_t maf : 1;
    uint8_t iacv : 1;
    uint8_t tps : 1;
    uint8_t oxygen : 1;
    uint8_t vss : 1;
    uint8_t purge : 1;
} trouble_code_two;

typedef struct
{
    uint8_t fuel_trim : 1;
    uint8_t idle_sw : 1;
    uint8_t unused_bit_2 : 1;
    uint8_t wgc : 1;
    uint8_t baro : 1;
    uint8_t wrong_maf : 1;
    uint8_t neutral_sw : 1;
    uint8_t parking_sw : 1;
} trouble_code_three;

#endif /* SSM1_H */
