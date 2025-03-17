#include "state_transition.h"
/*
const StateTransition transitions[] PROGMEM = {
    {SCAN_MODE, state.lcd_current_page = SENSOR_ONE, &handle_sensor_one},
    {SCAN_MODE, state.lcd_current_page = SENSOR_TWO, &handle_sensor_two},
    {SCAN_MODE, state.lcd_current_page = SENSOR_THREE, &handle_sensor_three},
    {SCAN_MODE, state.lcd_current_page = SENSOR_FOUR, &handle_sensor_four},
    {SCAN_MODE, state.lcd_current_page = SENSOR_FIVE, &handle_sensor_five},
    {SCAN_MODE, state.lcd_current_page = SENSOR_SIX, &handle_sensor_six},
    {SCAN_MODE, state.lcd_current_page = SENSOR_SEVEN, &handle_sensor_seven},
    {SCAN_MODE, state.lcd_current_page = SENSOR_EIGHT, &handle_sensor_eight},

    {PARAMS_MODE, state.param_current = PARAMS_ONE, &handle_sensor_one},
    {PARAMS_MODE, state.param_current = PARAMS_TWO, &handle_sensor_two},
    {PARAMS_MODE, state.param_current = PARAMS_THREE, &handle_sensor_three},
    {PARAMS_MODE, state.param_current = PARAMS_FOUR, &handle_sensor_four},
    {PARAMS_MODE, state.param_current = PARAMS_FIVE, &handle_sensor_five},
    {PARAMS_MODE, state.param_current = PARAMS_SIX, &handle_sensor_six},
    {PARAMS_MODE, state.param_current = PARAMS_SEV, &handle_sensor_seven},
    {PARAMS_MODE, state.param_current = PARAMS_EIGHT, &handle_sensor_eight},

    {DIAG_MODE, state.diag_current = IN, &handle_sensor_one},
    {DIAG_MODE, state.diag_current = IO, &handle_sensor_two},
    {DIAG_MODE, state.diag_current = AC_ONE, &handle_sensor_three},
    {DIAG_MODE, state.diag_current = AC_TWO, &handle_sensor_four},
    {DIAG_MODE, state.diag_current = AC_THREE, &handle_sensor_five},
    {DIAG_MODE, state.diag_current = SC_ONE, &handle_sensor_six},
    {DIAG_MODE, state.diag_current = SC_TWO, &handle_sensor_seven},
    {DIAG_MODE, state.diag_current = SC_THREE, &handle_sensor_eight},

    {RACE_MODE, state.race_current = TW, &handle_race_one},
    {RACE_MODE, state.race_current = TPS, &handle_race_two},
    {RACE_MODE, state.race_current = MP, &handle_race_three}
};
*/

void handle_scan_mode(void) 
{
    scan_page_control();
    execute_scan_page();
}

void handle_params_mode(void) 
{
    params_page_control();
    execute_params_page();
}

void handle_diag_mode(void) 
{
    diag_page_control();
    execute_diag_page();
}

void handle_race_mode(void) 
{
    race_page_control();
    execute_race_page();
}

void execute_scan_page(void) 
{
    switch (scan_curr_page) {
        case SCAN::SENSOR_ONE:       handle_sensor_one();    break;
        case SCAN::SENSOR_TWO:       handle_sensor_two();    break;
        case SCAN::SENSOR_THREE:     handle_sensor_three();  break;
        case SCAN::SENSOR_FOUR:      handle_sensor_four();   break;
        case SCAN::SENSOR_FIVE:      handle_sensor_five();   break;
        case SCAN::SENSOR_SIX:       handle_sensor_six();    break;
        case SCAN::SENSOR_SEVEN:     handle_sensor_seven();  break;
        case SCAN::SENSOR_EIGHT:     handle_sensor_eight();  break;
        case SCAN::INPUT_SW:         handle_input_sw();      break;
        case SCAN::IO_SW:            handle_io_sw();         break;
        case SCAN::ACTIVE_TC_ONE:    handle_active_one();    break;
        case SCAN::ACTIVE_TC_TWO:    handle_active_two();    break;
        case SCAN::ACTIVE_TC_THREE:  handle_active_three();  break;
        case SCAN::STORED_TC_ONE:    handle_stored_one();    break;
        case SCAN::STORED_TC_TWO:    handle_stored_two();    break;
        case SCAN::STORED_TC_THREE:  handle_stored_three();  break;
        case SCAN::CLEAR_TC:         handle_clear();         break;
    }
}

void execute_params_page(void) 
{
    switch (params_curr_page) {
        case PARAMS::PARAMS_ONE:    handle_sensor_one();    break;
        case PARAMS::PARAMS_TWO:    handle_sensor_two();    break;
        case PARAMS::PARAMS_THREE:  handle_sensor_three();  break;
        case PARAMS::PARAMS_FOUR:   handle_sensor_four();   break;
        case PARAMS::PARAMS_FIVE:   handle_sensor_five();   break;
        case PARAMS::PARAMS_SIX:    handle_sensor_six();    break;
        case PARAMS::PARAMS_SEV:    handle_sensor_seven();  break;
        case PARAMS::PARAMS_EIGHT:  handle_sensor_eight();  break;
    }
}

void execute_diag_page(void) 
{
    switch (diag_curr_page) {
        case DIAG::IN:        handle_input_sw();      break;
        case DIAG::IO:        handle_io_sw();         break;
        case DIAG::AC_ONE:    handle_active_one();    break;
        case DIAG::AC_TWO:    handle_active_two();    break;
        case DIAG::AC_THREE:  handle_active_three();  break;
        case DIAG::SC_ONE:    handle_stored_one();    break;
        case DIAG::SC_TWO:    handle_stored_two();    break;
        case DIAG::SC_THREE:  handle_stored_three();  break;
        case DIAG::CLEAR:     handle_clear();         break;
    }
}

void execute_race_page(void) 
{
    switch (race_curr_page) {
        case RACE::TW:   handle_race_one();     break;
        case RACE::TPS:  handle_race_two();     break;
        case RACE::MP:   handle_race_three();   break;
    }
}

void execute_state(void) 
{
    switch (menu_curr_page) {
        case MENU::SCAN_MODE:   handle_scan_mode();   break;
        case MENU::PARAMS_MODE: handle_params_mode(); break;
        case MENU::DIAG_MODE:   handle_diag_mode();   break;
        case MENU::RACE_MODE:   handle_race_mode();   break;
    }
}