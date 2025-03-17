#include "state_control.h"

void scan_page_control(void)
{
    if (btn1_pressed()) 
    {
        trigger_feedback_one();
        scan_curr_page = static_cast<SCAN>(static_cast<uint8_t>(scan_curr_page) + 1);
        if (scan_curr_page > SCAN::CLEAR_TC)
            scan_curr_page = SCAN::SENSOR_ONE;
    }
}

void diag_page_control(void)
{
    if (btn1_pressed()) 
    {
        trigger_feedback_one();
        diag_curr_page = static_cast<DIAG>(static_cast<uint8_t>(diag_curr_page) + 1);
        if (diag_curr_page > DIAG::CLEAR)
            diag_curr_page = DIAG::IN;
    }
}

void params_page_control(void)
{
    if (btn1_pressed())
    {
        trigger_feedback_one();
        params_curr_page = static_cast<PARAMS>(static_cast<uint8_t>(params_curr_page) + 1);
        if (params_curr_page > PARAMS::PARAMS_EIGHT)
            params_curr_page = PARAMS::PARAMS_ONE;
    }
}

void race_page_control(void)
{
        if(btn1_pressed())
        {
            trigger_feedback_one();
            race_curr_page = static_cast<RACE>(static_cast<uint8_t>(race_curr_page) + 1);
            if (race_curr_page > RACE::MP)
                race_curr_page = RACE::TW;
        }
}