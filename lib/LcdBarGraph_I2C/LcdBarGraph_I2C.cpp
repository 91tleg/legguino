#include "LcdBarGraph_I2C.h"

LcdBarGraph_I2C::LcdBarGraph_I2C(LiquidCrystal_I2C *lcd, uint8_t startX, uint8_t startY)
    : _lcd(lcd),
      _startX(startX),
      _startY(startY),
      _prevValue(0),
      _maxBarWidth(16 - startX) {}

void LcdBarGraph_I2C::begin()
{
    if (!_lcd)
        return;
    _lcd->createChar(1, level1);
    _lcd->createChar(2, level2);
    _lcd->createChar(3, level3);
    _lcd->createChar(4, level4);
}

void LcdBarGraph_I2C::drawValue(uint16_t value, uint16_t maxValue)
{
    if (value < 0 || value > maxValue)
        return;
    uint8_t fullBars = static_cast<uint8_t>(value * static_cast<uint16_t>(_maxBarWidth) / maxValue);
    uint8_t partialBar = static_cast<uint8_t>((value * static_cast<uint16_t>(_maxBarWidth * 5) / maxValue) % 5);
    uint8_t scaledValue = fullBars * 5 + partialBar;
    if (_prevValue != scaledValue)
    {
        _lcd->setCursor(_startX, _startY);
        // Print full bars
        for (uint8_t i = 0; i < fullBars; ++i)
        {
            _lcd->write(0XFF);
        }
        // Print partial bar
        if (partialBar > 0)
        {
            _lcd->write(partialBar);
            ++fullBars;
        }
        // Clear leftover characters
        for (uint8_t i = fullBars; i < _maxBarWidth; i++)
        {
            _lcd->write(static_cast<uint8_t>(' '));
        }
        _prevValue = scaledValue;
    }
}

void LcdBarGraph_I2C::drawValue(int16_t value, int16_t minValue, int16_t maxValue)
{
    if (minValue > maxValue || value < minValue || value > maxValue)
        return;
    uint8_t fullBars = static_cast<uint8_t>((static_cast<int32_t>(value - minValue) * _maxBarWidth) / (maxValue - minValue));
    uint8_t partialBar = static_cast<uint8_t>( (static_cast<int32_t>(value - minValue) * (_maxBarWidth * 5)) / (maxValue - minValue)) % 5;
    uint8_t scaledValue = fullBars * 5 + partialBar;
    uint8_t zeroPos = static_cast<uint8_t>((0 - minValue) * _maxBarWidth / (maxValue - minValue));

    _lcd->setCursor(zeroPos, _startY);
    if (fullBars < zeroPos)
        _lcd->write(1);

    if (_prevValue != scaledValue)
    {
        _lcd->setCursor(_startX, _startY);
        // Print full bars
        for (uint8_t i = 0; i < fullBars; ++i)
        {
            _lcd->write(0XFF);
        }
        // Print partial bar
        if (partialBar > 0)
        {
            _lcd->write(partialBar);
            ++fullBars;
        }
        // Clear leftover characters
        for (uint8_t i = fullBars; i < _maxBarWidth; ++i)
        {
            if (i == zeroPos)
                _lcd->write(1); // Don't clear zero mark
            else
                _lcd->write(static_cast<uint8_t>(' '));
        }
        _prevValue = scaledValue;
    }
}