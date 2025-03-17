#ifndef STATE_TRANSITION_H
#define STATE_TRANSITION_H

#include "state_handle.h"
#include "state.h"
/*
typedef struct
{
    uint8_t currMenu; // Defines which mode is active
    union
    {
        uint8_t lcd_current_page;
        uint8_t param_current;
        uint8_t diag_current;
        uint8_t race_current;
    } state;
} StateTransition;*/
 
void handle_scan_mode(void);
void handle_params_mode(void);
void handle_diag_mode(void);
void handle_race_mode(void);
void execute_scan_page(void);
void execute_params_page(void);
void execute_diag_page(void);
void execute_race_page(void);
void execute_state(void);

#endif // STATE_TRANSITION_H