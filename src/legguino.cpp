#include "legguino.h"

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLS, LCD_ROWS);
// BigNumbers_I2C bn(&lcd);
// LcdBarGraph lbg(&lcd, LCD_COLS, 0, 0);

extern MENU menu_current;
extern SCAN lcd_current_page;
extern PARAMS param_current;
extern DIAG diag_current;
extern RACE race_current;
extern ecu_params ecu_parameters;
extern input_switches status;
extern io_switches status0;
extern trouble_code_one status1;
extern trouble_code_two status2;
extern trouble_code_three status3;
extern uint8_t btn1;
extern uint8_t btn1_prev;
extern uint8_t btn2;
extern uint8_t btn2_prev;

void setup()
{
    DDRB |= (1 << PB7);
    lcd.init();
    lcd.backlight();
    btn_init();
    HWSerial.begin(1953, SERIAL_8E1);
    USBSerial.begin(9600);
#if PRINT_DEBUG_MESSAGES_ON_USB
    query_romid();
#endif

    // char cmd[4] = {0};
    // USBSerial.readBytes(cmd, 4);
    // if (!strncmp(cmd, "DUMP", 4))
    //{
    //   dump_address_space(0, 0xffff);
    // }
    uint8_t romid_buffer[3] = {0}; // Buffer to store rom id
    if (get_romid(romid_buffer))
    {
        for (uint8_t i = 0; i < 3; ++i)
        {
            ecu_parameters.romid_param[i] = romid_buffer[i];
        }
    }

    lcd.setCursor(0, 0);
    lcd.print(F("ROM ID: "));
    display_romid(&lcd);

    _delay_ms(5000);
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("SELECT MODE:");

    while (true)
    {
        btn_read();
        display_menu(menu_current, &lcd);
        // Button handling
        if (btn1 == LOW && btn1_prev == HIGH)
        {
            PORTB |= (1 << PB7);
            _delay_ms(100);
            PORTB &= ~(1 << PB7);
            menu_current = static_cast<MENU>(static_cast<uint8_t>(menu_current) + 1);

            // Return to page 0
            if (menu_current > MENU::COMPASS_MODE)
            {
                menu_current = MENU::SCAN_MODE;
            }
        }
        if (btn2 == LOW && btn2_prev == HIGH)
        {
            PORTB |= (1 << PB7);
            _delay_ms(100);
            PORTB &= ~(1 << PB7);
            lcd.clear();
            break;
        }
        btn1_prev = btn1;
        btn2_prev = btn2;
        _delay_ms(200);
    }

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
    if (lcd_current_page > SCAN::CLEAR_TC)
    {
        lcd_current_page = SCAN::SENSOR_ONE;
    }
    btn1 = digitalRead(BUTTON1_PIN);
    if (btn1 == LOW && btn1_prev == HIGH)
    {
        PORTB |= (1 << PB7);
        _delay_ms(500);
        PORTB &= ~(1 << PB7);
        lcd_current_page = static_cast<SCAN>(static_cast<uint8_t>(lcd_current_page) + 1);
    }
    btn1_prev = digitalRead(BUTTON1_PIN);

    switch (menu_current)
    {
    case MENU::SCAN_MODE:

        switch (lcd_current_page)
        {
        case SCAN::SENSOR_ONE:
            read_sensor_one();
            lcd.setCursor(0, 0);
            lcd.print("VB: ");
            lcd.print(ecu_parameters.vb);
            lcd.setCursor(8, 0);
            lcd.print("SPD:" + ecu_parameters.vsp);
            lcd.setCursor(0, 1);
            lcd.print("REV:" + ecu_parameters.erev);
            lcd.setCursor(8, 1);
            lcd.print("TW: " + ecu_parameters.tw);
            break;
        case SCAN::SENSOR_TWO:
            read_sensor_two();
            lcd.setCursor(0, 0);
            lcd.print("ADV:" + ecu_parameters.alphar);
            lcd.setCursor(8, 0);
            lcd.print("MAF:");
            lcd.print(ecu_parameters.qa);
            lcd.setCursor(0, 1);
            lcd.print("LD: " + ecu_parameters.ldata);
            lcd.setCursor(8, 1);
            lcd.print("TPS:" + ecu_parameters.tps);
            break;
        case SCAN::SENSOR_THREE:
            read_sensor_three();
            lcd.setCursor(0, 0);
            lcd.print("IPW:" + ecu_parameters.tim);
            lcd.setCursor(8, 0);
            lcd.print("ISC:" + ecu_parameters.isc);
            lcd.setCursor(0, 1);
            lcd.print("O2R:" + ecu_parameters.o2r);
            lcd.setCursor(8, 1);
            lcd.print("RTD:" + ecu_parameters.advs);
            break;
        case SCAN::SENSOR_FOUR:
            read_sensor_four();
            lcd.setCursor(0, 0);
            lcd.print("BAR:" + ecu_parameters.barop);
            lcd.setCursor(8, 0);
            lcd.print("MP: ");
            lcd.print(ecu_parameters.manip);
            lcd.setCursor(0, 1);
            lcd.print("WGC:" + ecu_parameters.wgc);
            lcd.setCursor(8, 1);
            lcd.print("THV:" + ecu_parameters.thv);
            break;
        case SCAN::INPUT_SW:
            read_input_switches();
            lcd.setCursor(0, 0);
            lcd.print("IG" + status.ignition);
            lcd.setCursor(4, 0);
            lcd.print("AT" + status.auto_trans);
            lcd.setCursor(8, 0);
            lcd.print("TM" + status.test_mode);
            lcd.setCursor(12, 0);
            lcd.print("RM" + status.read_mode);
            lcd.setCursor(0, 1);
            lcd.print("NT" + status.neutral);
            lcd.setCursor(4, 1);
            lcd.print("PK" + status.park);
            lcd.setCursor(8, 1);
            lcd.print("CA" + status.california);
            break;
        case SCAN::IO_SW:
            read_io_switches();
            lcd.setCursor(0, 0);
            lcd.print("ID" + status0.idle_sw);
            lcd.setCursor(4, 0);
            lcd.print("AC" + status0.ac_sw);
            lcd.setCursor(8, 0);
            lcd.print("AR" + status0.ac_relay);
            lcd.setCursor(12, 0);
            lcd.print("RF" + status0.rad_fan);
            lcd.setCursor(0, 1);
            lcd.print("FP" + status0.fuel_pump);
            lcd.setCursor(4, 1);
            lcd.print("CN" + status0.purge_valve);
            lcd.setCursor(8, 1);
            lcd.print("KS" + status0.pinging);
            lcd.setCursor(12, 1);
            lcd.print("PX" + status0.press_exch);
            break;
        case SCAN::ACTIVE_TC_ONE:
            read_active_trouble_code_one();
            lcd.setCursor(0, 0);
            lcd.print(11);
            lcd.print(status1.crank);
            lcd.setCursor(4, 0);
            lcd.print(12);
            lcd.print(status1.starter);
            lcd.setCursor(8, 0);
            lcd.print(13);
            lcd.print(status1.cam);
            lcd.setCursor(12, 0);
            lcd.print(14);
            lcd.print(status1.inj_1);
            lcd.setCursor(0, 1);
            lcd.print(15);
            lcd.print(status1.inj_2);
            lcd.setCursor(4, 1);
            lcd.print(16);
            lcd.print(status1.inj_3);
            lcd.setCursor(8, 1);
            lcd.print(17);
            lcd.print(status1.inj_4);
            break;
        case SCAN::ACTIVE_TC_TWO:
            read_active_trouble_code_two();
            lcd.setCursor(0, 0);
            lcd.print(18);
            lcd.print(status2.temp);
            lcd.setCursor(4, 0);
            lcd.print(22);
            lcd.print(status2.knock);
            lcd.setCursor(8, 0);
            lcd.print(23);
            lcd.print(status2.maf);
            lcd.setCursor(12, 0);
            lcd.print(45);
            lcd.print(status2.iacv);
            lcd.setCursor(0, 1);
            lcd.print(31);
            lcd.print(status2.tps);
            lcd.setCursor(4, 1);
            lcd.print(32);
            lcd.print(status2.oxygen);
            lcd.setCursor(8, 1);
            lcd.print(33);
            lcd.print(status2.vss);
            lcd.setCursor(12, 1);
            lcd.print(35);
            lcd.print(status2.purge);
            break;
        case SCAN::ACTIVE_TC_THREE:
            read_active_trouble_code_three();
            lcd.setCursor(0, 0);
            lcd.print(41);
            lcd.print(status3.fuel_trim);
            lcd.setCursor(4, 0);
            lcd.print(42);
            lcd.print(status3.idle_sw);
            lcd.setCursor(8, 0);
            lcd.print(44);
            lcd.print(status3.wgc);
            lcd.setCursor(12, 0);
            lcd.print(45);
            lcd.print(status3.baro);
            lcd.setCursor(0, 1);
            lcd.print(49);
            lcd.print(status3.wrong_maf);
            lcd.setCursor(4, 1);
            lcd.print(51);
            lcd.print(status3.neutral_sw);
            lcd.setCursor(8, 1);
            lcd.print(52);
            lcd.print(status3.parking_sw);
            break;
        case SCAN::STORED_TC_ONE:
            read_stored_trouble_code_one();
            lcd.setCursor(0, 0);
            lcd.print(11);
            lcd.print(status1.crank);
            lcd.setCursor(4, 0);
            lcd.print(12);
            lcd.print(status1.starter);
            lcd.setCursor(8, 0);
            lcd.print(13);
            lcd.print(status1.cam);
            lcd.setCursor(12, 0);
            lcd.print(14);
            lcd.print(status1.inj_1);
            lcd.setCursor(0, 1);
            lcd.print(15);
            lcd.print(status1.inj_2);
            lcd.setCursor(4, 1);
            lcd.print(16);
            lcd.print(status1.inj_3);
            lcd.setCursor(8, 1);
            lcd.print(17);
            lcd.print(status1.inj_4);
            break;
        case SCAN::STORED_TC_TWO:
            read_stored_trouble_code_two();
            lcd.setCursor(0, 0);
            lcd.print(21);
            lcd.print(status2.temp);
            lcd.setCursor(4, 0);
            lcd.print(22);
            lcd.print(status2.knock);
            lcd.setCursor(8, 0);
            lcd.print(23);
            lcd.print(status2.maf);
            lcd.setCursor(12, 0);
            lcd.print(45);
            lcd.print(status2.iacv);
            lcd.setCursor(0, 1);
            lcd.print(31);
            lcd.print(status2.tps);
            lcd.setCursor(4, 1);
            lcd.print(32);
            lcd.print(status2.oxygen);
            lcd.setCursor(8, 1);
            lcd.print(33);
            lcd.print(status2.vss);
            lcd.setCursor(12, 1);
            lcd.print(35);
            lcd.print(status2.purge);
            break;
        case SCAN::STORED_TC_THREE:
            read_stored_trouble_code_three();
            lcd.setCursor(0, 0);
            lcd.print(41);
            lcd.print(status3.fuel_trim);
            lcd.setCursor(4, 0);
            lcd.print(42);
            lcd.print(status3.idle_sw);
            lcd.setCursor(8, 0);
            lcd.print(44);
            lcd.print(status3.wgc);
            lcd.setCursor(12, 0);
            lcd.print(45);
            lcd.print(status3.baro);
            lcd.setCursor(0, 1);
            lcd.print(49);
            lcd.print(status3.wrong_maf);
            lcd.setCursor(4, 1);
            lcd.print(51);
            lcd.print(status3.neutral_sw);
            lcd.setCursor(8, 1);
            lcd.print(52);
            lcd.print(status3.parking_sw);
            break;
        case SCAN::CLEAR_TC:
            lcd.setCursor(2, 0);
            lcd.print("CLEAR CODES?");
            btn2 = digitalRead(BUTTON2_PIN);
            if (btn2 == LOW && btn2_prev == HIGH)
            {
                lcd.setCursor(0, 0);
                lcd.print("CLEARING........");
                PORTB |= (1 << PB7);
                _delay_ms(1000);
                PORTB &= ~(1 << PB7);
                lcd_current_page = static_cast<SCAN>(static_cast<uint8_t>(lcd_current_page) + 1);
                lcd.clear();
            }
            _delay_ms(100);
            btn2_prev = digitalRead(BUTTON2_PIN);
            break;
        }
        _delay_ms(150);
        lcd.clear();
        break;

    case MENU::PARAMS_MODE:
        switch (param_current)
        {
        case PARAMS::PARAMS_ONE:
            lcd.setCursor(0, 0);
            //lcd.print("VBAT " + ecu_parameters.vb);
            lcd.setCursor(8, 0);
            lcd.print("VSPD " + ecu_parameters.vsp);
            break;
        case PARAMS::PARAMS_TWO:
            lcd.setCursor(0, 1);
            lcd.print("REV:" + ecu_parameters.erev);
            lcd.setCursor(8, 1);
            lcd.print("WATR: " + ecu_parameters.tw);
            break;
        case PARAMS::PARAMS_THREE:
            lcd.setCursor(0, 0);
            lcd.print("ADV:" + ecu_parameters.alphar);
            lcd.setCursor(8, 0);
            //lcd.print("MAF:" + ecu_parameters.qa);
            break;
        case PARAMS::PARAMS_FOUR:
            lcd.setCursor(0, 1);
            lcd.print("LD: " + ecu_parameters.ldata);
            lcd.setCursor(8, 1);
            lcd.print("TPS:" + ecu_parameters.tps);
            break;
        case PARAMS::PARAMS_FIVE:
            lcd.setCursor(0, 0);
            lcd.print("IPW:" + ecu_parameters.tim);
            lcd.setCursor(8, 0);
            lcd.print("ISC:" + ecu_parameters.isc);
            break;
        case PARAMS::PARAMS_SIX:
            lcd.setCursor(0, 1);
            lcd.print("O2R:" + ecu_parameters.o2r);
            lcd.setCursor(8, 1);
            lcd.print("RTD:" + ecu_parameters.advs);
            break;
        case PARAMS::PARAMS_SEV:
            lcd.setCursor(0, 0);
            lcd.print("BAR:" + ecu_parameters.barop);
            lcd.setCursor(8, 0);
            //lcd.print("MP: " + ecu_parameters.manip);
            break;
        case PARAMS::PARAMS_EIGHT:
            lcd.setCursor(0, 1);
            lcd.print("WGC:" + ecu_parameters.wgc);
            lcd.setCursor(8, 1);
            lcd.print("THV:" + ecu_parameters.thv);
            break;
        }
        break;

    case MENU::DIAG_MODE:
        switch (diag_current)
        {
        case DIAG::IN:
            break;
        case DIAG::IO:
            break;
        case DIAG::AC_ONE:
            break;
        case DIAG::AC_TWO:
            break;
        case DIAG::AC_THREE:
            break;
        case DIAG::SC_ONE:
            break;
        case DIAG::SC_TWO:
            break;
        case DIAG::SC_THREE:
            break;
        case DIAG::CLEAR:
            break;
        }
        break;

    case MENU::RACE_MODE:
        switch (race_current)
        {
        case RACE::TW:
            break;
        case RACE::TPS:
            break;
        case RACE::MP:
            break;
        case RACE::RPM:
            break;
        case RACE::VSP:
            break;
        }
        break;
    case MENU::COMPASS_MODE:
        break;
    }
}
#pragma GCC diagnostic pop