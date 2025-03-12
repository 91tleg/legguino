
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
#include <stdint.h>
#include "btn.h"
#include "addresses.h"
#include "ssm1.h"
#include "utilities.h"
#include "state_menu.h"
#include "state_scan.h"
#include "state_params.h"
#include "state_diag.h"
#include "state_race.h"
#include "state_handle.h"
#include "parameters.h"
#include "state.h"
#include "state_transition.h"

#define USBSerial Serial
#define HWSerial Serial

void setup();
void loop();

#endif // LEGGUINO_H
