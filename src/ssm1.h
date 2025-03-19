/*
 * http://www.4bc.org/vanagon/SSM_params.html
 * http://www.alcyone.org.uk/ssm/ecureverse.html
 * https://github.com/P1kachu/ssm1-gc8
 */

#ifndef SSM1_H
#define SSM1_H

#include <avr/pgmspace.h>
#include <stdint.h>
#include "legguino.h"
#include "addresses.h"
#include "parameters.h"

/* ====================================== PROTOCOL ====================================== */
/* 12	00	 00  00	    Stop                                                                
 * 78	msb lsb 00	    Read data from ECU address
 * AA	msb lsb data    Write data to address
 * AB	00	 lsb	    Read data from Cruise address
 * 00	46	 48  49	    Get ROM ID
 * Reply format:        msb  lsb  data
 */

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


void send_clear_command(uint16_t addr);
/*
 * AA MSB LSB 00   (Clear to Zero)
 * AA MSB LSB FF   (If inverted value is used)
 * In order to write data, you must be receiving data. ie. To write FF to address 1234 in the ECU, you would first
 * issue an ECU read command 78123400 and then a write command AA1234FF.
 */

#endif // SSM1_H
