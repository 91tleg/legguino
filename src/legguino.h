#ifndef LEGGUINO_H
#define LEGGUINO_H

#include <Arduino.h>
#include <stdint.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <avr/io.h>
#include <util/delay.h>
#include "ssm1.h"
#include "utilities.h"
#include "reverse_engineering.h"
/*
 *  Brown cable: Pin 0 (Serial RX --> Subaru TX --> OBD-II 13)
 *  Red cable:   Pin 1 (Serial TX --> Subaru RX --> OBD-II 12)
 */
#define HWSerial Serial1
#define USBSerial Serial
#define PRINT_DEBUG_MESSAGES_ON_USB 0
#define TEST 0
#define BUTTON1_PIN 31
#define BUTTON2_PIN 32
#define LED_PIN 13
#define rs A3
#define en A5
#define d4 A9
#define d5 A10
#define d6 A11
#define d7 A12
constexpr uint8_t LCD_MAX_PAGE_COUNT = 13;

void setup();
void loop();

// To avoid the --unused-variable flag
#ifndef UNUSED_VARIABLE
#define UNUSED_VARIABLE(x) (void)(x)
#endif

#endif /* LEGGUINO_H */
