#ifndef UTILITIES_H
#define UTILITIES_H

//#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LcdBarGraph_I2C.h>

#if !defined(UNUSED)
#define UNUSED(x) (void)(x)
#endif

void print_received_data(uint8_t answer[3]);
void trigger_feedback_one(void);
void trigger_feedback_two(void);

extern LiquidCrystal_I2C lcd;
extern LcdBarGraph_I2C lbg;
extern char buffer[]; // Buffer for lcd

#endif // UTILITIES_H