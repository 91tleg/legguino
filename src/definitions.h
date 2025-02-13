#ifndef PINDEF_H
#define PINDEF_H

#define HWSerial Serial1
#define USBSerial Serial

#define BUTTON1_PIN 22
#define BUTTON2_PIN 24

#define GY521_SCL A1
#define GY521_SDA A0

#define I2C_ADDR 0x27
#define I2C_C_ADDR 0x68

#define LCD
#undef LCD

#ifdef LCD
#define rs A3
#define en A5
#define d4 A9
#define d5 A10
#define d6 A11
#define d7 A12
#endif

#define LCD_I2C

#define LCD_COLS 16
#define LCD_ROWS 2
//#define LCD_MAX_PAGE_COUNT 13

#define MENU_MAX_PAGES 5
#endif