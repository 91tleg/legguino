#ifndef LEGGUINO_H
#define LEGGUINO_H

#include <Arduino.h>
#include <stdint.h>
#include <LiquidCrystal.h>
#include <avr/io.h>
#include <util/delay.h>
#include "pindef.h"
#include "ssm1.h"
#include "utilities.h"
#include "reverse_engineering.h"
#include "sensors.h"
#include "sensors_state.h"
#include "lcd/lcd.h"
#include "dbg.h"

/*    Yellow 9-pin diagnosis connector:    */
/* Green/white cable:      Pin 2  transmit */
/* Light green/red cable:  Pin 3  recieve  */
/* Black/red cable:        Pin 9  ground   */

#define HWSerial Serial1
#define USBSerial Serial
#define TEST 0

void setup();
void loop();

// To avoid the --unused-variable flag
#ifndef UNUSED_VARIABLE
#define UNUSED_VARIABLE(x) (void)(x)
#endif

#endif /* LEGGUINO_H */
