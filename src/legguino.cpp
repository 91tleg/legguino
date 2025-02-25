#include "legguino.h"

char buffer[32];

LiquidCrystal_I2C lcd(0x27, 16, 2);
// LcdBarGraph lbg(&lcd, LCD_COLS, 0, 0);

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

    lcd.setCursor(0, 0);
    uint8_t romid_buffer[3] = {0}; // Buffer to store rom id
    if (get_romid(romid_buffer))
    {
        for (uint8_t i = 0; i < 3; ++i)
        {
            ecu_parameters.romid_param[i] = romid_buffer[i];
        }
        lcd.print("ROM ID: ");
        display_romid();
    }
    else
    {
        lcd.print(F("ERROR"));
    }
    _delay_ms(5000);
    lcd.setCursor(0, 0);
    lcd.print("  SELECT MODE:");
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

    switch (menu_current)
    {
    case MENU::SCAN_MODE:
        if (lcd_current_page > SCAN::CLEAR_TC)
            lcd_current_page = SCAN::SENSOR_ONE;
        btn1 = digitalRead(BUTTON1_PIN);
        if (btn1 == LOW && btn1_prev == HIGH)
        {
            PORTB |= (1 << PB7);
            _delay_ms(100);
            PORTB &= ~(1 << PB7);
            lcd_current_page = static_cast<SCAN>(static_cast<uint8_t>(lcd_current_page) + 1);
        }
        btn1_prev = digitalRead(BUTTON1_PIN);
        switch (lcd_current_page)
        {
        case SCAN::SENSOR_ONE:
            // read_sensor_one();
            lcd.setCursor(0, 0);
            lcd.print("VB:");
            lcd.print(ecu_parameters.vb, 1);
            lcd.setCursor(8, 0);
            sprintf(buffer, "SPD:%d", ecu_parameters.vsp);
            lcd.print(buffer);
            lcd.setCursor(0, 1);
            sprintf(buffer, "R:%d     TW:%d", ecu_parameters.erev, ecu_parameters.tw);
            lcd.print(buffer);
            break;
        case SCAN::SENSOR_TWO:
            // read_sensor_two();
            lcd.setCursor(0, 0);
            lcd.print("ADV:");
            lcd.print(ecu_parameters.alphar, 1);
            lcd.setCursor(8, 0);
            lcd.print("MAF:");
            lcd.print(ecu_parameters.qa, 1);
            lcd.setCursor(0, 1);
            sprintf(buffer, "LD:%d    TPS:%d", ecu_parameters.ldata, ecu_parameters.tps);
            lcd.print(buffer);
            break;
        case SCAN::SENSOR_THREE:
            // read_sensor_three();
            lcd.setCursor(0, 0);
            lcd.print("IPW:");
            lcd.print(ecu_parameters.tim, 1);
            lcd.setCursor(8, 0);
            lcd.print("O2R:");
            lcd.print(ecu_parameters.o2r, 1);
            lcd.setCursor(0, 1);
            sprintf(buffer, "ISC:%d   RTD:%d", ecu_parameters.isc, ecu_parameters.advs);
            lcd.print(buffer);
            break;
        case SCAN::SENSOR_FOUR:
            // read_sensor_four();
            lcd.setCursor(0, 0);
            lcd.print("BAR:");
            lcd.print(ecu_parameters.barop, 1);
            lcd.setCursor(8, 0);
            lcd.print("MP:");
            lcd.print(ecu_parameters.manip, 1);
            lcd.setCursor(0, 1);
            lcd.print("WGC:");
            lcd.print(ecu_parameters.wgc, 1);
            lcd.setCursor(8, 1);
            lcd.print("THV:");
            lcd.print(ecu_parameters.thv, 1);
            break;
        case SCAN::INPUT_SW:
            // read_input_switches();
            sprintf(buffer, "IG%d AT%d TM%d RM%d", status.ignition, status.auto_trans, status.test_mode, status.read_mode);
            lcd.setCursor(0, 0);
            lcd.print(buffer);
            sprintf(buffer, "NT%d PK%d CA%d     ", status.neutral, status.park, status.california);
            lcd.setCursor(0, 1);
            lcd.print(buffer);
            break;
        case SCAN::IO_SW:
            // read_io_switches();
            sprintf(buffer, "ID%d AC%d AR%d RF%d", status0.idle_sw, status0.ac_sw, status0.ac_relay, status0.rad_fan);
            lcd.setCursor(0, 0);
            lcd.print(buffer);
            sprintf(buffer, "FP%d CN%d KS%d PX%d", status0.fuel_pump, status0.purge_valve, status0.pinging, status0.press_exch);
            lcd.setCursor(0, 1);
            lcd.print(buffer);
            break;
        case SCAN::ACTIVE_TC_ONE:
            // read_active_trouble_code_one();
            sprintf(buffer, "11%d 12%d 13%d 14%d", status1.crank, status1.starter, status1.cam, status1.inj_1);
            lcd.setCursor(0, 0);
            lcd.print(buffer);
            sprintf(buffer, "15%d 16%d 17%d", status1.inj_2, status1.inj_3, status1.inj_4);
            lcd.setCursor(0, 1);
            lcd.print(buffer);
            break;
        case SCAN::ACTIVE_TC_TWO:
            // read_active_trouble_code_two();
            sprintf(buffer, "21%d 22%d 23%d 24%d", status2.temp, status2.knock, status2.maf, status2.iacv);
            lcd.setCursor(0, 0);
            lcd.print(buffer);
            sprintf(buffer, "31%d 32%d 33%d 35%d", status2.tps, status2.oxygen, status2.vss, status2.purge);
            lcd.setCursor(0, 1);
            lcd.print(buffer);
            break;
        case SCAN::ACTIVE_TC_THREE:
            // read_active_trouble_code_three();
            sprintf(buffer, "41%d 42%d 44%d 45%d", status3.fuel_trim, status3.idle_sw, status3.wgc, status3.baro);
            lcd.setCursor(0, 0);
            lcd.print(buffer);
            sprintf(buffer, "49%d 51%d 52%d", status3.wrong_maf, status3.neutral_sw, status3.parking_sw);
            lcd.setCursor(0, 1);
            lcd.print(buffer);
            break;
        case SCAN::STORED_TC_ONE:
            // read_stored_trouble_code_one();
            sprintf(buffer, "11%d 12%d 13%d 14%d", status1.crank, status1.starter, status1.cam, status1.inj_1);
            lcd.setCursor(0, 0);
            lcd.print(buffer);
            sprintf(buffer, "15%d 16%d 17%d", status1.inj_2, status1.inj_3, status1.inj_4);
            lcd.setCursor(0, 1);
            lcd.print(buffer);
            break;
        case SCAN::STORED_TC_TWO:
            // read_stored_trouble_code_two();
            sprintf(buffer, "21%d 22%d 23%d 24%d", status2.temp, status2.knock, status2.maf, status2.iacv);
            lcd.setCursor(0, 0);
            lcd.print(buffer);
            sprintf(buffer, "31%d 32%d 33%d 35%d", status2.tps, status2.oxygen, status2.vss, status2.purge);
            lcd.setCursor(0, 1);
            lcd.print(buffer);
            break;
        case SCAN::STORED_TC_THREE:
            // read_stored_trouble_code_three();
            sprintf(buffer, "41%d 42%d 44%d 45%d", status3.fuel_trim, status3.idle_sw, status3.wgc, status3.baro);
            lcd.setCursor(0, 0);
            lcd.print(buffer);
            sprintf(buffer, "49%d 51%d 52%d", status3.wrong_maf, status3.neutral_sw, status3.parking_sw);
            lcd.setCursor(0, 1);
            lcd.print(buffer);
            break;
        case SCAN::CLEAR_TC:
            lcd.clear();
            lcd.setCursor(2, 0);
            lcd.print("CLEAR CODES?");
            btn2 = digitalRead(BUTTON2_PIN);
            if (btn2 == LOW && btn2_prev == HIGH)
            {
                lcd.setCursor(0, 0);
                lcd.print("CLEARING........");
                // CLEAR
                // CLEAR
                // CLEAR
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
        break;

    case MENU::PARAMS_MODE:
        if (param_current > PARAMS::PARAMS_ONE)
            param_current = PARAMS::PARAMS_EIGHT;
        btn1 = digitalRead(BUTTON1_PIN);
        if (btn1 == LOW && btn1_prev == HIGH)
        {
            PORTB |= (1 << PB7);
            _delay_ms(100);
            PORTB &= ~(1 << PB7);
            param_current = static_cast<PARAMS>(static_cast<uint8_t>(param_current) + 1);
        }
        btn1_prev = digitalRead(BUTTON1_PIN);
        switch (param_current)
        {
        case PARAMS::PARAMS_ONE:
            // read_sensor_one();
            lcd.setCursor(0, 0);
            lcd.print("VBAT:");
            lcd.print(ecu_parameters.vb, 1);
            lcd.setCursor(0, 1);
            sprintf(buffer, "VSPD:%dmph", ecu_parameters.vsp);
            lcd.print(buffer);
            break;
        case PARAMS::PARAMS_TWO:
            // read_sensor_one();
            lcd.setCursor(0, 0);
            sprintf(buffer, "REV:%d   ", ecu_parameters.erev);
            lcd.print(buffer);
            lcd.setCursor(0, 1);
            sprintf(buffer, "WATR:%df  ", ecu_parameters.tw);
            lcd.print(buffer);
            break;
        case PARAMS::PARAMS_THREE:
            // read_sensor_two();
            lcd.setCursor(0, 0);
            lcd.print("ADVS:");
            lcd.print(ecu_parameters.advs, 1);
            lcd.print(buffer);
            lcd.setCursor(0, 1);
            lcd.print("MAF:");
            lcd.print(ecu_parameters.qa, 1);
            break;
        case PARAMS::PARAMS_FOUR:
            // read_sensor_two();
            lcd.setCursor(0, 0);
            sprintf(buffer, "LOAD:%d   ", ecu_parameters.ldata);
            lcd.print(buffer);
            lcd.setCursor(0, 1);
            sprintf(buffer, "TPS:%d%%  ", ecu_parameters.tps);
            lcd.print(buffer);
            break;
        case PARAMS::PARAMS_FIVE:
            // read_sensor_three();
            lcd.setCursor(0, 0);
            lcd.print("INJPW:");
            lcd.print(ecu_parameters.tim, 1);
            lcd.setCursor(0, 1);
            sprintf(buffer, "ISC:%d%% ", ecu_parameters.isc);
            lcd.print(buffer);
            break;
        case PARAMS::PARAMS_SIX:
            lcd.setCursor(0, 0);
            lcd.print("OXY:");
            lcd.print(ecu_parameters.o2r, 1);
            lcd.setCursor(0, 1);
            sprintf(buffer, "ADVS:%d%%", ecu_parameters.advs);
            lcd.print(buffer);
            break;
        case PARAMS::PARAMS_SEV:
            lcd.setCursor(0, 0);
            lcd.print("BARO:");
            lcd.print(ecu_parameters.barop, 1);
            lcd.setCursor(0, 1);
            lcd.print("BOOST:");
            lcd.print(ecu_parameters.manip, 1);
            break;
        case PARAMS::PARAMS_EIGHT:
            lcd.setCursor(0, 0);
            lcd.print("WGC:");
            lcd.print(ecu_parameters.wgc, 1);
            lcd.setCursor(0, 1);
            lcd.print("THV:");
            lcd.print(ecu_parameters.thv, 1);
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