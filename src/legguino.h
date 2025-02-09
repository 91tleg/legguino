#ifndef LEGGUINO_H
#define LEGGUINO_H

#include <Arduino.h>
#include <stdint.h>
#include <LiquidCrystal.h>
#include <LcdBarGraph.h>
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
#include "btn.h"

/*    Yellow 9-pin diagnosis connector:    */
/* Green/white cable:      Pin 2  transmit */
/* Light green/red cable:  Pin 3  recieve  */
/* Black/red cable:        Pin 9  ground   */

void setup();
void loop();

#endif /* LEGGUINO_H */
