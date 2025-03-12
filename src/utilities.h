#ifndef UTILITIES_H
#define UTILITIES_H

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <LcdBarGraph_I2C.h>

/* Set bit b (0-7) in byte a */
#define BIT_SET(var,pos) ((var) |= (1U<<(pos)))
/* Clear bit b (0-7) in byte a */
#define BIT_CLEAR(var,pos) ((var) &= ~(1U<<(pos)))

#if !defined(UNUSED)
#define UNUSED(x) (void)(x)
#endif

void print_received_data(uint8_t answer[3]);

extern LiquidCrystal_I2C lcd;
extern LcdBarGraph_I2C lbg;
extern char buffer[]; // Buffer for lcd

#endif // UTILITIES_H