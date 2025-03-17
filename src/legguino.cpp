#include "legguino.h"

void setup()
{
    DDRB |= (1 << PB7);
    lcd.init();
    lcd.backlight();
    lbg.begin();
    btn1_init();
    btn2_init();

    // HWSerial.begin(1953, SERIAL_8E1);
    /*
    UBRR1 = 0x0672;  // 1953 baud
    UCSR1A &= ~(1 << U2X1);  // Disable double speed mode
    UCSR1C = (1 << UPM11);   // Even parity, 8-bit data
    UCSR1B = (1 << RXEN1) | (1 << TXEN1); // Enable receiver and transmitter
    */

    HWSerial.begin(9600, SERIAL_8N1); // Python

    lcd.setCursor(0, 0);
    lcd.print("rom id: ");
    uint8_t romid_buffer[3] = {0};
    get_romid(romid_buffer);
    print_received_data(romid_buffer);
    _delay_ms(3000);
    select_mode();
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"

void __attribute__((always_inline)) loop()
{
    check_return_menu();
    execute_state();
    /*
    check_return_menu();
    switch (menu_curr_page)
    {
    case MENU::SCAN_MODE:
        scan_page_control();
        switch (scan_curr_page)
        {
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
        break;
    case MENU::PARAMS_MODE:
        params_page_control();
        switch (params_curr_page)
        {
        case PARAMS::PARAMS_ONE:    handle_sensor_one();    break;
        case PARAMS::PARAMS_TWO:    handle_sensor_two();    break;
        case PARAMS::PARAMS_THREE:  handle_sensor_three();  break;
        case PARAMS::PARAMS_FOUR:   handle_sensor_four();   break;
        case PARAMS::PARAMS_FIVE:   handle_sensor_five();   break;
        case PARAMS::PARAMS_SIX:    handle_sensor_six();    break;
        case PARAMS::PARAMS_SEV:    handle_sensor_seven();  break;
        case PARAMS::PARAMS_EIGHT:  handle_sensor_eight();  break;
        default:
            break;
        }
        break;
    case MENU::DIAG_MODE:
        diag_page_control();
        switch (diag_curr_page)
        {
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
        break;
    case MENU::RACE_MODE:
        race_page_control();
        switch (race_curr_page)
        {
        case RACE::TW:   handle_race_one();  break;
        case RACE::TPS:  handle_race_two();  break;
        case RACE::MP:   handle_race_three();break;
        }
        break;
    }*/
   //_delay_ms(100); // Delay to avoid sending requests too fast
   delay_millis(100);
}
#pragma GCC diagnostic pop