
/*    Yellow 9-pin diagnosis connector:    */
/* Green/white cable:      Pin 2  transmit */
/* Light green/red cable:  Pin 3  recieve  */
/* Black/red cable:        Pin 9  ground   */

#ifndef LEGGUINO_H
#define LEGGUINO_H

#include <Arduino.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "state_menu.h"
#include "state_execute.h"

#define USBSerial Serial
#define HWSerial Serial

void setup();
void loop();

#endif // LEGGUINO_H
