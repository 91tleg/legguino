#ifndef LEGGUINO_H
#define LEGGUINO_H

#include <Arduino.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <BigNumbers_I2C.h>
#include "btn.h"
#include "definitions.h"
#include "addresses.h"
#include "ssm1.h"
#include "utilities.h"
#include "reverse_engineering.h"
#include "state_menu.h"
#include "state_scan.h"
#include "state_params.h"
#include "state_diag.h"
#include "state_race.h"

/*    Yellow 9-pin diagnosis connector:    */
/* Green/white cable:      Pin 2  transmit */
/* Light green/red cable:  Pin 3  recieve  */
/* Black/red cable:        Pin 9  ground   */

/*
12	00	00	00	Stop
78	msb	lsb	00	Read data from ECU address
45	msb	lsb	00	Read data from TCU address
89	00	lsb		Read data from AirCon address
92	00	lsb		Read data from 4WS address
AA	msb	lsb	data	Write data to address
AB	00	lsb		Read data from Cruise address
00	46	48	49	Get ROM ID

Reply format:
msb	lsb	data

*/

void setup();
void loop();

#endif // LEGGUINO_H
