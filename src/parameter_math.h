#ifndef PARAMETER_MATH_H
#define PARAMETER_MATH_H

#include <stdint.h>
#include "ssm1.h"
#include "addresses.h"
#include "parameters.h"

constexpr uint8_t coolant_look_up_table[256] PROGMEM =
{
		// the first 14 values need 255 added
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
		// the last 29 values need to be inverted
		               2,  4,  6,  8,  9,  9, 11, 13, 15, 17, 18, 20, 22,
		  24, 27, 31, 33, 36, 40, 42, 45, 47, 47, 47, 47, 47, 47, 47, 47
};

void read_battery_voltage(void); // x*0.08    V
/*
 * Voltage measured at the ECU.
 */

void read_speed(void);           // x/1.6    mph
/* 
 * Speed measured by the vehicle speed sensor.
 */

void read_rpm(void);             // x*25    rev / min
/*
 * RPM measured by cam and crank angle sensors.
 */

void read_coolant_temp(void);    // look_up_table(x)    deg F
void read_airflow(void);         // x/50   V
/*
 * MAF measures the amount of air being used for the air/fuel mixture.
 */

void read_throttle_percentage(void);
void read_throttle_signal(void);    // V
/* 
 * How much the throttle is open.
 */

void read_manifold_pressure(void);        // x/0.128-1060
void read_boost_control_duty_cycle(void); // x/2.56
void read_ignition_timing(void);          // x   deg BTDC
/* 
 * Degrees BTDC that the spark is fired.
 * this is one of the primary engine control parameters (outputs of the ECU)
 * - Timing is altered if knocking is detected, or angular velocity of the crankshaft fluctuates (engine is jerking)
 */

void read_load(void);                     // x    air mass/rev
/*
 * The engine load is essentially the mass of air consumed per revolution of the engine. 
 * The greater the mass per revolution, the greater demand on the engine for output power.
 * - Calculated using MAF (mass/time) sensor and RPM (rev/time) from cam/crank sensors:
 *   (mass / time) / (rev / time) = mass / rev
 */

void read_injector_pulse_width(void);     // x*0.128 ms
/*
 * IPW is the time that the ECU keeps each injector open during the intake stroke.
 * - Primarily dependent upon the engine load.
 * - A larger mass of air requires a larger amount of fuel to maintain a stochiometric ratio.
 * - Other corrections are applied at different conditions, e.g. starting (increased for rich mixture).
 */

void read_iacv_duty_cycle(void);          // x/2   % duty cycle
/*
 * The amount of air allowed to bypass the throttle for idle.
 * - Measured as a percent of the time that the solenoid is open.
 * - Continuously opened and closed at a variable rate to control the amount of air.
 * - Higher duty ratio -> valve is open longer -> higher idle
 * - Non-zero even when not idling; but insignificant when compared to the air entering from open throttle.
 * Compensates for idle speed decreases when:
 *  - AC operation / electrical loads.
 *  - Prevent rich air/fuel during deceleration.
 *  - Mechanically compensate for engine temperature.
 */

void read_o2_signal(void);                // x/100   V
/*
 * Lower values (towards 0) -> lean mixture
 * Higher (towards 1) -> rich mixture
 */

void read_timing_correction(void);        // x   deg
/* 
 * The degrees of timing advance added to timing to correct for knock
 * - Changed to non-zero when pinging is detected by the knock sensor
 * - Used to temporarily advance (often only for several seconds) the timing to counteract pinging
 */

void read_fuel_trim(void);                // (x-128)/1.28
/*
 * The increment applied to the fuel injection to bring the air/fuel mixture (as sensed by the 
 * oxygen sensor) closer to the stochiometric ratio.
 * - 0.1V - 0.5V (lean mixture) -> a/f correction is positive
 * - 0.5V - 0.9V (rich mixture) -> a/f correction is negative
 * - The longer the mixture stays rich or lean, the greater the correction is applied
 */

void read_atmosphere_pressure(void);      // x*1.25+500

void read_input_switches(void);
void read_io_switches(void);
void read_trouble_code_one(uint16_t addr);
void read_active_trouble_code_one(void);
void read_stored_trouble_code_one(void);
void read_trouble_code_two(uint16_t addr);
void read_active_trouble_code_two(void);
void read_stored_trouble_code_two(void);
void read_trouble_code_three(uint16_t addr);
void read_active_trouble_code_three(void);
void read_stored_trouble_code_three(void);

#endif