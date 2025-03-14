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

#ifndef STATE_DIAG_H
#define STATE_DIAG_H

#include <Arduino.h>
#include "state.h"
#include "btn.h"

void diag_page_control(void);
void lcd_print_input_sw(void);
void lcd_print_io_sw(void);
void lcd_print_code_one(void);
void lcd_print_code_two(void);
void lcd_print_code_three(void);
void check_clear_code(void);

#endif // STATE_DIAG_H