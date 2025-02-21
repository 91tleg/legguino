#ifndef ECU_PARAMS_H
#define ECU_PARAMS_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "ssm1.h"

enum class PARAMS : uint8_t
{
  /* 00 */ PARAMS_ONE,
  /* 01 */ PARAMS_TWO,
  /* 02 */ PARAMS_THREE,
  /* 03 */ PARAMS_FOUR,
  /* 04 */ PARAMS_FIVE,
  /* 05 */ PARAMS_SIX,
  /* 06 */ PARAMS_SEV,
  /* 07 */ PARAMS_EIGHT
};

#pragma pack(push, 1)
typedef struct 
{
    uint8_t romid_param[3];
    uint8_t ldata;
    float vb;
    uint8_t vsp;
    uint16_t erev;
    int16_t tw;
    uint16_t advs;
    float qa;
    uint8_t thv;
    uint8_t tps;
    uint16_t tim;
    uint16_t isc;
    uint16_t o2r;
    uint16_t alphar;
    float manip;
    uint16_t rtrd;
    uint16_t wgc;
    uint16_t barop;
} ecu_params;
#pragma pack(pop)

constexpr uint8_t coolant_look_up_table[256] PROGMEM = 
{
    // The first 14 values need 255 added
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
                  2,  4,  6,  8,  9,  9, 11, 13, 15, 17, 18, 20, 22,
     24, 27, 31, 33, 36, 40, 42, 45, 47, 47, 47, 47, 47, 47, 47, 47   
    // The last 29 values need to be inverted (to negative)
};

void display_romid(LiquidCrystal_I2C *lcd);
void read_sensor_one(void);
void read_sensor_two(void);
void read_sensor_three(void);
void read_sensor_four(void);

#endif // STATE_PARAMS_H