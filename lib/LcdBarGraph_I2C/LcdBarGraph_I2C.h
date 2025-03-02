#ifndef LCDBARGRAPH_H
#define LCDBARGRAPH_H

#include <Arduino.h>
#include <stdint.h>
#include <LiquidCrystal_I2C.h>

class LcdBarGraph_I2C
{
public:
    LcdBarGraph_I2C(LiquidCrystal_I2C *, uint8_t, uint8_t);
    void begin();
    void drawValue(uint16_t, uint16_t);
    void drawValue(int16_t, int16_t, int16_t);

private:
    LiquidCrystal_I2C *_lcd;
    uint8_t _startX;
    uint8_t _startY;
    int16_t _prevValue;
    uint8_t _maxBarWidth;

    uint8_t level1[8] = {
        B10000,
        B10000,
        B10000,
        B10000,
        B10000,
        B10000,
        B10000,
        B10000};

    uint8_t level2[8] = {
        B11000,
        B11000,
        B11000,
        B11000,
        B11000,
        B11000,
        B11000,
        B11000};

    uint8_t level3[8] = {
        B11100,
        B11100,
        B11100,
        B11100,
        B11100,
        B11100,
        B11100,
        B11100};

    uint8_t level4[8] = {
        B11110,
        B11110,
        B11110,
        B11110,
        B11110,
        B11110,
        B11110,
        B11110};
};

#endif