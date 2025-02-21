#ifndef STATE_DIAG_H
#define STATE_DIAG_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

enum class DIAG : uint8_t
{
    /* 00 */ IN,
    /* 01 */ IO,
    /* 02 */ AC_ONE,
    /* 03 */ AC_TWO,
    /* 04 */ AC_THREE,
    /* 05 */ SC_ONE,
    /* 06 */ SC_TWO,
    /* 07 */ SC_THREE,
    /* 08 */ CLEAR
};

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

/*
 * The current codes are stored as bit flags in locations 0x0047 through 0x0049 and the previous codes are stored
 * in locations 0x1604 through 0x1606.
 *
 * -INPUT SWITCH:
 *            Bit7	        Bit6	         Bit5	    Bit4	    Bit3	     Bit2	       Bit1	         Bit0
 * 0x15A8     Igniton       AutoTrans        TestMode   ReadMode    n/a          N-switch      P-switch      California
 *
 * -IO SWITCH:
 *            Bit7	        Bit6	         Bit5	    Bit4	    Bit3	     Bit2	       Bit1	         Bit0
 * 0x15A8     Idle-switch   AC-switch        AC-relay   RadFan      FuelPump     Purge         Pinging       Pressure-xchng
 *
 * -ACTIVE CODE:
 *            Bit7	        Bit6	         Bit5	    Bit4	    Bit3	     Bit2	       Bit1	         Bit0
 * 0x0047	  Crank(11)	    Starter(12)      Cam(13)	Inj1(14)    Inj2(15)     Inj3(16)      Inj4(17)      n/a
 * 0x0048	  Temp(21)	    Knock(22)        MAF(23)    IAC(24)     TPS(31)      Oxy(32)       VSS(33)	     Purge(35)
 * 0x0049	  FuelTrim(41)  Idle-switch(42)  n/a        WGC(44)     Baro(45)     WrongMAF(49)  N-switch(51)  P-switch(52)
 *
 * -STORED CODE:
 *            Bit7	        Bit6	         Bit5	    Bit4	    Bit3	     Bit2	       Bit1	         Bit0
 * 0x1604	  Crank(11)	    Starter(12)      Cam(13)	Inj1(14)    Inj2(15)     Inj3(16)      Inj4(17)      n/a
 * 0x1605	  Temp(21)	    Knock(22)        MAF(23)    IAC(24)     TPS(31)      Oxy(32)       VSS(33)	     Purge(35)
 * 0x1606	  FuelTrim(41)  Idle-switch(42)  n/a        WGC(44)     Baro(45)     WrongMAF(49)  N-switch(51)  P-switch(52)
 *
 * As an example, suppose location 0x0047 contains the value 0x48. That is 01001000 in binary. This indicates
 * errors with the Starter (Code 12) and Injector 2 (Code 15).
 */

#endif // STATE_DIAG_H