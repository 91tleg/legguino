#include "legguino.h"

char buffer[32];

LiquidCrystal_I2C lcd(0x27, 16, 2);
LcdBarGraph_I2C lbg(&lcd, 0, 1);

void setup()
{
    DDRB |= (1 << PB7);
    lcd.init();
    lcd.backlight();
    lbg.begin();
    btn_init();
    HWSerial.begin(1953, SERIAL_8E1);
    USBSerial.begin(9600);
#if PRINT_DEBUG_MESSAGES_ON_USB
    query_romid();
#endif

    lcd.setCursor(0, 0);
    uint8_t romid_buffer[3] = {0}; // Buffer to store rom id
    if (get_romid(romid_buffer))
    {
        for (uint8_t i = 0; i < 3; ++i)
        {
            ecu_parameters.romid_param[i] = romid_buffer[i];
        }
        lcd.print("rom id: ");
        display_romid();
    }
    else
    {
        lcd.print(F("rom id error "));
    }
    _delay_ms(5000);
    lcd.setCursor(0, 0);
    lcd.print("select mode:");
    select_mode();
#if TEST
    int index = 0;
    int values[] = {1, 7, 8, 9, 10, 11, 12, 13, 14};
    while (true)
    {
        for (int i = 0; i < 10000; ++i)
        {
            monitor_special_addresses(values[index]);
        }

        index++;

        int arr_size = sizeof(values) / sizeof(values[0]);
        if (index > arr_size - 1)
        {
            index = 0;
        }
    }
#endif
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
void __attribute__((always_inline)) loop()
{
    check_return_menu();
    switch (menu_current)
    {
    case MENU::SCAN_MODE:
        scan_page_control();
        switch (lcd_current_page)
        {
        case SCAN::SENSOR_ONE:
            // read_sensor_one();
            lcd_print_params_one();
            break;
        case SCAN::SENSOR_TWO:
            // read_sensor_two();
            lcd_print_params_two();
            break;
        case SCAN::SENSOR_THREE:
            // read_sensor_three();
            lcd_print_params_three();
            break;
        case SCAN::SENSOR_FOUR:
            // read_sensor_four();
            lcd_print_params_four();
            break;
        case SCAN::SENSOR_FIVE:
            // read_sensor_five();
            lcd_print_params_five();
            break;
        case SCAN::SENSOR_SIX:
            // read_sensor_six();
            lcd_print_params_six();
            break;
        case SCAN::SENSOR_SEVEN:
            // read_sensor_seven();
            lcd_print_params_seven();
            break;
        case SCAN::SENSOR_EIGHT:
            // read_sensor_eight();
            lcd_print_params_eight();
            break;
        case SCAN::INPUT_SW:
            // read_input_switches();
            lcd_print_input_sw();
            break;
        case SCAN::IO_SW:
            // read_io_switches();
            lcd_print_io_sw();
            break;
        case SCAN::ACTIVE_TC_ONE:
            // read_active_trouble_code_one();
            lcd_print_code_one();
            break;
        case SCAN::ACTIVE_TC_TWO:
            // read_active_trouble_code_two();
            lcd_print_code_two();
            break;
        case SCAN::ACTIVE_TC_THREE:
            // read_active_trouble_code_three();
            lcd_print_code_three();
            break;
        case SCAN::STORED_TC_ONE:
            // read_stored_trouble_code_one();
            lcd_print_code_one();
            break;
        case SCAN::STORED_TC_TWO:
            // read_stored_trouble_code_two();
            lcd_print_code_two();
            break;
        case SCAN::STORED_TC_THREE:
            // read_stored_trouble_code_three();
            lcd_print_code_three();
            break;
        case SCAN::CLEAR_TC:
            lcd.setCursor(0, 0);
            lcd.print("CLEAR CODES?    ");
            lcd.setCursor(0, 1);
            lcd.print("                ");
            btn2 = digitalRead(BUTTON2_PIN);
            if (btn2 == LOW && btn2_prev == HIGH)
            {
                lcd.setCursor(0, 0);
                lcd.print("CLEARING........");
                int i = 0;
                while (i <= 100)
                {
                    lbg.drawValue(i, 100);
                    _delay_ms(50);
                    ++i;
                }
                _delay_ms(1000);
                lcd.clear();
                lcd_current_page = static_cast<SCAN>(static_cast<uint8_t>(lcd_current_page) + 1);
            }
            _delay_ms(100);
            btn2_prev = digitalRead(BUTTON2_PIN);
            break;
        }
        break;

    case MENU::PARAMS_MODE:
        params_page_control();
        switch (param_current)
        {
        case PARAMS::PARAMS_ONE:
            // read_sensor_one();
            lcd_print_params_one();
            break;
        case PARAMS::PARAMS_TWO:
            // read_sensor_two();
            lcd_print_params_two();
            break;
        case PARAMS::PARAMS_THREE:
            // read_sensor_three();
            lcd_print_params_three();
            break;
        case PARAMS::PARAMS_FOUR:
            // read_sensor_four();
            lcd_print_params_four();
            break;
        case PARAMS::PARAMS_FIVE:
            // read_sensor_five();
            lcd_print_params_five();
            break;
        case PARAMS::PARAMS_SIX:
            // read_sensor_six();
            lcd_print_params_six();
            break;
        case PARAMS::PARAMS_SEV:
            // read_sensor_seven();
            lcd_print_params_seven();
            break;
        case PARAMS::PARAMS_EIGHT:
            // read_sensor_eight();
            lcd_print_params_eight();
            break;
        }
        break;

    case MENU::DIAG_MODE:
        switch (diag_current)
        {
        case DIAG::IN:
            // read_input_switches();
            lcd_print_input_sw();
            break;
        case DIAG::IO:
            // read_io_switches();
            lcd_print_io_sw();
            break;
        case DIAG::AC_ONE:
            // read_active_trouble_code_one();
            lcd_print_code_one();
            break;
        case DIAG::AC_TWO:
            // read_active_trouble_code_two();
            lcd_print_code_two();
            break;
        case DIAG::AC_THREE:
            // read_active_trouble_code_three();
            lcd_print_code_three();
            break;
        case DIAG::SC_ONE:
            // read_active_trouble_code_one();
            lcd_print_code_one();
            break;
        case DIAG::SC_TWO:
            // read_active_trouble_code_two();
            lcd_print_code_two();
            break;
        case DIAG::SC_THREE:
            // read_active_trouble_code_three();
            lcd_print_code_three();
            break;
        case DIAG::CLEAR:
            break;
        }
        break;

    case MENU::RACE_MODE:
        race_page_control();
        switch (race_current)
        {
        case RACE::TW:
            // read_coolant_temp();
            lcd.setCursor(0, 0);
            sprintf(buffer, "Temp W:%df    ", ecu_parameters.tw);
            lcd.print(buffer);
            lbg.drawValue(ecu_parameters.tw, 275);
            break;
        case RACE::TPS:
            // read_throttle_percentage();
            lcd.setCursor(0, 0);
            sprintf(buffer, "Throttle:%d%%", ecu_parameters.tps);
            lcd.print(buffer);
            lbg.drawValue(ecu_parameters.tps, 100);
            break;
        case RACE::MP:
            // read_manifold_pressure();
            lcd.setCursor(0, 0);
            lcd.print("Boost:");
            lcd.print(ecu_parameters.manip, 2);
            if (ecu_parameters.manip < 0)
                lcd.print("inHG  ");
            else
                lcd.print("psi");
            lbg.drawValue(static_cast<int16_t>(ecu_parameters.manip), -25, 20);
            break;
        }
        break;
    }
}
#pragma GCC diagnostic pop