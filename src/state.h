#ifndef STATE_H
#define STATE_H

#include <stdint.h>

enum class MENU : uint8_t
{
    /* 00 */ SCAN_MODE,
    /* 01 */ PARAMS_MODE,
    /* 02 */ DIAG_MODE,
    /* 03 */ RACE_MODE
};

enum class SCAN : uint8_t
{
    /* 00 */ SENSOR_ONE,
    /* 01 */ SENSOR_TWO,
    /* 02 */ SENSOR_THREE,
    /* 03 */ SENSOR_FOUR,
    /* 04 */ SENSOR_FIVE,
    /* 05 */ SENSOR_SIX,
    /* 06 */ SENSOR_SEVEN,
    /* 07 */ SENSOR_EIGHT,
    /* 08 */ INPUT_SW,
    /* 09 */ IO_SW,
    /* 10 */ ACTIVE_TC_ONE,
    /* 11 */ ACTIVE_TC_TWO,
    /* 12 */ ACTIVE_TC_THREE,
    /* 13 */ STORED_TC_ONE,
    /* 14 */ STORED_TC_TWO,
    /* 15 */ STORED_TC_THREE,
    /* 16 */ CLEAR_TC
};

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

enum class DIAG : uint8_t
{
    /* 00 */ IN,
    /* 01 */ IO,
    /* 02 */ AC_ONE,
    /* 03 */ AC_TWO,
    /* 04 */ AC_THREE,
    /* 05 */ SC_ONE,
    /* 06 */ SC_TWO,
    /* 07 */ SC_THREE,
    /* 08 */ CLEAR
}; 

enum class RACE : uint8_t
{
    /* 00 */ TW,
    /* 01 */ TPS,
    /* 02 */ MP
};

extern MENU menu_curr_page;
extern SCAN scan_curr_page;
extern PARAMS params_curr_page;
extern DIAG diag_curr_page;
extern RACE race_curr_page;

#endif