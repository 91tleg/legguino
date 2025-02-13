#include "legguino.h"

#ifdef LCD
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
LcdBarGraph lbg(&lcd, NUM_COLS, 0, 0);
#endif

#ifdef LCD_I2C
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLS, LCD_ROWS);
BigNumbers bn(&lcd);
#endif

extern SCAN lcd_current_page;
extern MENU menu_current;
extern PARAMS param_current;
extern DIAG diag_current;
extern RACE race_current;
extern BIG big_current;

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
  lcd.init();
  lcd.backlight();
  btn_init();
  //read_btns();

  bn.begin();

  DDRB |= (1 << PB7);

  HWSerial.begin(1953, SERIAL_8E1);

  // Serial to send data over USB
  // USBSerial.begin(9600);

  // While no command was received, wait (reader over USB init)
  /*while (!USBSerial.available())
  {
    PORTB |= (1 << 5);  // LED ON
    for (uint32_t i = 0; i < 4000000; ++i) { __asm__ volatile("nop"); }
    PORTB &= ~(1 << 5); // LED OFF
    for (uint32_t i = 0; i < 4000000; ++i) { __asm__ volatile("nop"); }
  }
  stop_read();*/

#if PRINT_DEBUG_MESSAGES_ON_USB
  query_romid();
#endif

  // char cmd[4] = {0};
  // USBSerial.readBytes(cmd, 4);
  //  USBSerial.clear();
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
  lcd.print(F("ROM ID:"));
  for (uint8_t i = 0; i < 3; ++i)
  {
    if (ecu_parameters.romid_param[i] < 0x10)
    {
      lcd.print("0"); // Print leading zero for single-digit hex values
    }
    lcd.print(ecu_parameters.romid_param[i], HEX);
    if (i < 2)
    {
      lcd.print(".");
    }
  }
  _delay_ms(5000);
  lcd.clear();

  lcd.setCursor(5, 0);
  lcd.print("MODE:");
  _delay_ms(2000);
  lcd.clear();

  while (true)
  {
    btn1 = digitalRead(BUTTON1_PIN);
    btn2 = digitalRead(BUTTON2_PIN);
    switch (menu_current)
    {
    case MENU::SCAN_MODE:
      lcd.setCursor(3, 1);
      lcd.print("SCAN MODE");
      break;
    case MENU::PARAMS_MODE:
      lcd.setCursor(3, 1);
      lcd.print("PARAMS MODE");
      break;
    case MENU::DIAG_MODE:
      lcd.setCursor(3, 1);
      lcd.print("DIAG MODE");
      break;
    case MENU::RACE_MODE:
      lcd.setCursor(3, 1);
      lcd.print("RACE MODE");
      break;
    case MENU::PARAMS_BIG:
      lcd.setCursor(2, 1);
      lcd.print("PARAMS BIG");
      break;
    case MENU::COMPASS_MODE:
      lcd.setCursor(2, 1);
      lcd.print("COMPASS MODE");
      break;
    default:
      break;
    }
    // Button handling
    if (btn1 == LOW && btn1_prev == HIGH)
    {
      PORTB |= (1 << PB7);
      _delay_ms(100);
      PORTB &= ~(1 << PB7);
      menu_current = static_cast<MENU>((static_cast<uint8_t>(menu_current) + 1) % static_cast<uint8_t>(MENU::COMPASS_MODE) + 1);
      lcd.clear();
    }
    if (btn2 == LOW && btn2_prev == HIGH) {
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
  _delay_ms(1000);
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

#if PRINT_DEBUG_MESSAGES_ON_USB
USBSerial.println("Setup done");
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
    PORTB |= (1 << PB7); // Turn on built in led
    lcd_current_page = static_cast<SCAN>(static_cast<uint8_t>(lcd_current_page) + 1);
    _delay_ms(500);
    PORTB &= ~(1 << PB7);
  }
  btn1_prev = digitalRead(BUTTON1_PIN);

  switch (menu_current)
  {
  case MENU::SCAN_MODE:

    switch (lcd_current_page)
    {
    case SCAN::SENSOR_ONE:
      // read_battery_voltage();
      lcd.setCursor(0, 0);
      lcd.print(F("VB: "));
      lcd.print(ecu_parameters.vb);

      // read_speed();
      lcd.setCursor(8, 0);
      lcd.print(F("SPD:"));
      lcd.print(ecu_parameters.vsp);

      // read_rpm();
      lcd.setCursor(0, 1);
      lcd.print(F("REV:"));
      lcd.print(ecu_parameters.erev);

      // read_coolant_temp();
      lcd.setCursor(8, 1);
      lcd.print(F("TW: "));
      lcd.print(ecu_parameters.tw);
      break;

    case SCAN::SENSOR_TWO:
      // read_fuel_trim();
      lcd.setCursor(0, 0);
      lcd.print(F("ADV:"));
      lcd.print(ecu_parameters.alphar);

      // read_airflow();
      lcd.setCursor(8, 0);
      lcd.print(F("MAF:"));
      lcd.print(ecu_parameters.qa);

      // read_load();
      lcd.setCursor(0, 1);
      lcd.print(F("LD: "));
      lcd.print(ecu_parameters.ldata);

      // read_throttle_percentage();
      lcd.setCursor(8, 1);
      lcd.print(F("TPS:"));
      lcd.print(ecu_parameters.tps);
      break;

    case SCAN::SENSOR_THREE:
      // read_injector_pulse_width();
      lcd.setCursor(0, 0);
      lcd.print(F("IPW:"));
      lcd.print(ecu_parameters.tim);

      // read_iacv_duty_cycle();
      lcd.setCursor(8, 0);
      lcd.print(F("ISC:"));
      lcd.print(ecu_parameters.isc);

      // read_o2_signal();
      lcd.setCursor(0, 1);
      lcd.print(F("O2R:"));
      lcd.print(ecu_parameters.o2r);

      // read_timing_correction();
      lcd.setCursor(8, 1);
      lcd.print(F("RTD:"));
      lcd.print(ecu_parameters.advs);
      break;

    case SCAN::SENSOR_FOUR:
      // read_atmosphere_pressure();
      lcd.setCursor(0, 0);
      lcd.print(F("BAR:"));
      lcd.print(ecu_parameters.barop);

      // read_manifold_pressure();
      lcd.setCursor(8, 0);
      lcd.print(F("MP: "));
      lcd.print(ecu_parameters.manip);

      // read_boost_control_duty_cycle();
      lcd.setCursor(0, 1);
      lcd.print(F("WGC:"));
      lcd.print(ecu_parameters.wgc);

      // read_throttle_signal();
      lcd.setCursor(8, 1);
      lcd.print(F("THV:"));
      lcd.print(ecu_parameters.thv);
      break;

    case SCAN::INPUT_SW:
      // read_input_switches();
      lcd.setCursor(0, 0);
      lcd.print(F("IG"));
      lcd.print(status.ignition);

      lcd.setCursor(4, 0);
      lcd.print(F("AT"));
      lcd.print(status.auto_trans);

      lcd.setCursor(8, 0);
      lcd.print(F("TM"));
      lcd.print(status.test_mode);

      lcd.setCursor(12, 0);
      lcd.print(F("RM"));
      lcd.print(status.read_mode);

      lcd.setCursor(0, 1);
      lcd.print(F("NT"));
      lcd.print(status.neutral);

      lcd.setCursor(4, 1);
      lcd.print(F("PK"));
      lcd.print(status.park);

      lcd.setCursor(8, 1);
      lcd.print(F("CA"));
      lcd.print(status.california);
      break;

    case SCAN::IO_SW:
      // read_io_switches();
      lcd.setCursor(0, 0);
      lcd.print(F("ID"));
      lcd.print(status0.idle_sw);

      lcd.setCursor(4, 0);
      lcd.print(F("AC"));
      lcd.print(status0.ac_sw);

      lcd.setCursor(8, 0);
      lcd.print(F("AR"));
      lcd.print(status0.ac_relay);

      lcd.setCursor(12, 0);
      lcd.print(F("RF"));
      lcd.print(status0.rad_fan);

      lcd.setCursor(0, 1);
      lcd.print(F("FP"));
      lcd.print(status0.fuel_pump);

      lcd.setCursor(4, 1);
      lcd.print(F("CN"));
      lcd.print(status0.purge_valve);

      lcd.setCursor(8, 1);
      lcd.print(F("KS"));
      lcd.print(status0.pinging);

      lcd.setCursor(12, 1);
      lcd.print(F("PX"));
      lcd.print(status0.press_exch);
      break;

    case SCAN::ACTIVE_TC_ONE:
      // read_active_trouble_code_one();
      lcd.setCursor(0, 0);
      lcd.print((uint8_t)11);
      lcd.print(status1.crank);

      lcd.setCursor(4, 0);
      lcd.print((uint8_t)12);
      lcd.print(status1.starter);

      lcd.setCursor(8, 0);
      lcd.print((uint8_t)13);
      lcd.print(status1.cam);

      lcd.setCursor(12, 0);
      lcd.print((uint8_t)14);
      lcd.print(status1.inj_1);

      lcd.setCursor(0, 1);
      lcd.print((uint8_t)15);
      lcd.print(status1.inj_2);

      lcd.setCursor(4, 1);
      lcd.print((uint8_t)16);
      lcd.print(status1.inj_3);

      lcd.setCursor(8, 1);
      lcd.print((uint8_t)17);
      lcd.print(status1.inj_4);
      break;

    case SCAN::ACTIVE_TC_TWO:
      // read_active_trouble_code_two();
      lcd.setCursor(0, 0);
      lcd.print((uint8_t)18);
      lcd.print(status2.temp);

      lcd.setCursor(4, 0);
      lcd.print((uint8_t)22);
      lcd.print(status2.knock);

      lcd.setCursor(8, 0);
      lcd.print((uint8_t)23);
      lcd.print(status2.maf);

      lcd.setCursor(12, 0);
      lcd.print((uint8_t)45);
      lcd.print(status2.iacv);

      lcd.setCursor(0, 1);
      lcd.print((uint8_t)31);
      lcd.print(status2.tps);

      lcd.setCursor(4, 1);
      lcd.print((uint8_t)32);
      lcd.print(status2.oxygen);

      lcd.setCursor(8, 1);
      lcd.print((uint8_t)33);
      lcd.print(status2.vss);

      lcd.setCursor(12, 1);
      lcd.print((uint8_t)35);
      lcd.print(status2.purge);
      break;

    case SCAN::ACTIVE_TC_THREE:
      // read_active_trouble_code_three();
      lcd.setCursor(0, 0);
      lcd.print((uint8_t)41);
      lcd.print(status3.fuel_trim);

      lcd.setCursor(4, 0);
      lcd.print((uint8_t)42);
      lcd.print(status3.idle_sw);

      lcd.setCursor(8, 0);
      lcd.print((uint8_t)44);
      lcd.print(status3.wgc);

      lcd.setCursor(12, 0);
      lcd.print((uint8_t)45);
      lcd.print(status3.baro);

      lcd.setCursor(0, 1);
      lcd.print((uint8_t)49);
      lcd.print(status3.wrong_maf);

      lcd.setCursor(4, 1);
      lcd.print((uint8_t)51);
      lcd.print(status3.neutral_sw);

      lcd.setCursor(8, 1);
      lcd.print((uint8_t)52);
      lcd.print(status3.parking_sw);
      break;

    case SCAN::STORED_TC_ONE:
      // read_stored_trouble_code_one();
      lcd.setCursor(0, 0);
      lcd.print((uint8_t)11);
      lcd.print(status1.crank);

      lcd.setCursor(4, 0);
      lcd.print((uint8_t)12);
      lcd.print(status1.starter);

      lcd.setCursor(8, 0);
      lcd.print((uint8_t)13);
      lcd.print(status1.cam);

      lcd.setCursor(12, 0);
      lcd.print((uint8_t)14);
      lcd.print(status1.inj_1);

      lcd.setCursor(0, 1);
      lcd.print((uint8_t)15);
      lcd.print(status1.inj_2);

      lcd.setCursor(4, 1);
      lcd.print((uint8_t)16);
      lcd.print(status1.inj_3);

      lcd.setCursor(8, 1);
      lcd.print((uint8_t)17);
      lcd.print(status1.inj_4);
      break;

    case SCAN::STORED_TC_TWO:
      // read_stored_trouble_code_two();
      lcd.setCursor(0, 0);
      lcd.print((uint8_t)21);
      lcd.print(status2.temp);

      lcd.setCursor(4, 0);
      lcd.print((uint8_t)22);
      lcd.print(status2.knock);

      lcd.setCursor(8, 0);
      lcd.print((uint8_t)23);
      lcd.print(status2.maf);

      lcd.setCursor(12, 0);
      lcd.print((uint8_t)45);
      lcd.print(status2.iacv);

      lcd.setCursor(0, 1);
      lcd.print((uint8_t)31);
      lcd.print(status2.tps);

      lcd.setCursor(4, 1);
      lcd.print((uint8_t)32);
      lcd.print(status2.oxygen);

      lcd.setCursor(8, 1);
      lcd.print((uint8_t)33);
      lcd.print(status2.vss);

      lcd.setCursor(12, 1);
      lcd.print((uint8_t)35);
      lcd.print(status2.purge);
      break;

    case SCAN::STORED_TC_THREE:
      // read_stored_trouble_code_three();
      lcd.setCursor(0, 0);
      lcd.print((uint8_t)41);
      lcd.print(status3.fuel_trim);

      lcd.setCursor(4, 0);
      lcd.print((uint8_t)42);
      lcd.print(status3.idle_sw);

      lcd.setCursor(8, 0);
      lcd.print((uint8_t)44);
      lcd.print(status3.wgc);

      lcd.setCursor(12, 0);
      lcd.print((uint8_t)45);
      lcd.print(status3.baro);

      lcd.setCursor(0, 1);
      lcd.print((uint8_t)49);
      lcd.print(status3.wrong_maf);

      lcd.setCursor(4, 1);
      lcd.print((uint8_t)51);
      lcd.print(status3.neutral_sw);

      lcd.setCursor(8, 1);
      lcd.print((uint8_t)52);
      lcd.print(status3.parking_sw);
      break;

    case SCAN::CLEAR_TC:

      lcd.setCursor(2, 0);
      lcd.print("CLEAR CODES?");

      btn2 = digitalRead(BUTTON2_PIN);
      if (btn2 == LOW && btn2_prev == HIGH)
      {
        PORTB |= (1 << PB7);
        lcd.clear();
        lcd.print("CLEARING........");
        _delay_ms(100);
        lcd.clear();
        /*
        // lcd bar graph
        lbg.drawValue(0, 16);
        _delay_ms(500);
        lbg.drawValue(1, 16);
        for (uint8_t i = 0; i <= 3; ++i)
        {
          // send_clear_command(ACTIVE_TROUBLE_CODE_THREE_ADDR, ACTIVE_TROUBLE_CODE_THREE_ADDR);
          _delay_ms(200);
        }
        lbg.drawValue(2, 16);
        _delay_ms(500);
        lbg.drawValue(3, 16);
        _delay_ms(500);
        lbg.drawValue(4, 16);
        _delay_ms(500);
        lbg.drawValue(5, 16);
        _delay_ms(500);
        lbg.drawValue(6, 16);
        _delay_ms(500);
        lbg.drawValue(7, 16);
        for (uint8_t i = 0; i <= 3; ++i)
        {
          // send_clear_command(STORED_TROUBLE_CODE_THREE_ADDR, STORED_TROUBLE_CODE_ONE_ADDR);
          _delay_ms(200);
        }
        lbg.drawValue(8, 16);
        _delay_ms(500);
        lbg.drawValue(9, 16);
        _delay_ms(500);
        lbg.drawValue(10, 16);
        _delay_ms(500);
        lbg.drawValue(11, 16);
        _delay_ms(500);
        lbg.drawValue(12, 16);
        _delay_ms(500);
        lbg.drawValue(13, 16);
        _delay_ms(500);
        lbg.drawValue(14, 16);
        _delay_ms(1000);
        lbg.drawValue(15, 16);
        _delay_ms(1000);
        lbg.drawValue(16, 16);
        _delay_ms(1000);
        lcd.clear();
        */
        PORTB &= ~(1 << PB7);
        // lcd_current_page++;
        lcd_current_page = static_cast<SCAN>(static_cast<uint8_t>(lcd_current_page) + 1);
      }
      _delay_ms(100);
      btn2_prev = digitalRead(BUTTON2_PIN);
      break;

    default:
      break;
    }
    _delay_ms(150);
    lcd.clear();
    break;

  case MENU::PARAMS_MODE:
    switch (param_current)
    {
    case PARAMS::PARAMS_ONE:
      // read_battery_voltage();
      lcd.setCursor(0, 0);
      lcd.print("VBAT ");
      lcd.print(ecu_parameters.vb);

      // read_speed();
      lcd.setCursor(8, 0);
      lcd.print(F("VSPD "));
      lcd.print(ecu_parameters.vsp);
      break;

    case PARAMS::PARAMS_TWO:
      // read_rpm();
      lcd.setCursor(0, 1);
      lcd.print(F("REV:"));
      lcd.print(ecu_parameters.erev);

      // read_coolant_temp();
      lcd.setCursor(8, 1);
      lcd.print(F("WATR: "));
      lcd.print(ecu_parameters.tw);
      break;

    case PARAMS::PARAMS_THREE:
      // read_fuel_trim();
      lcd.setCursor(0, 0);
      lcd.print(F("ADV:"));
      lcd.print(ecu_parameters.alphar);

      // read_airflow();
      lcd.setCursor(8, 0);
      lcd.print(F("MAF:"));
      lcd.print(ecu_parameters.qa);
      break;

    case PARAMS::PARAMS_FOUR:
      // read_load();
      lcd.setCursor(0, 1);
      lcd.print(F("LD: "));
      lcd.print(ecu_parameters.ldata);

      // read_throttle_percentage();
      lcd.setCursor(8, 1);
      lcd.print(F("TPS:"));
      lcd.print(ecu_parameters.tps);
      break;

    case PARAMS::PARAMS_FIVE:
      // read_injector_pulse_width();
      lcd.setCursor(0, 0);
      lcd.print(F("IPW:"));
      lcd.print(ecu_parameters.tim);

      // read_iacv_duty_cycle();
      lcd.setCursor(8, 0);
      lcd.print(F("ISC:"));
      lcd.print(ecu_parameters.isc);
      break;

    case PARAMS::PARAMS_SIX:
      // read_o2_signal();
      lcd.setCursor(0, 1);
      lcd.print(F("O2R:"));
      lcd.print(ecu_parameters.o2r);

      // read_timing_correction();
      lcd.setCursor(8, 1);
      lcd.print(F("RTD:"));
      lcd.print(ecu_parameters.advs);
      break;

    case PARAMS::PARAMS_SEV:
      // read_atmosphere_pressure();
      lcd.setCursor(0, 0);
      lcd.print(F("BAR:"));
      lcd.print(ecu_parameters.barop);

      // read_manifold_pressure();
      lcd.setCursor(8, 0);
      lcd.print(F("MP: "));
      lcd.print(ecu_parameters.manip);
      break;

    case PARAMS::PARAMS_EIGHT:
      // read_boost_control_duty_cycle();
      lcd.setCursor(0, 1);
      lcd.print(F("WGC:"));
      lcd.print(ecu_parameters.wgc);

      // read_throttle_signal();
      lcd.setCursor(8, 1);
      lcd.print(F("THV:"));
      lcd.print(ecu_parameters.thv);
      break;

    default:
      break;
    }
    break;

  case MENU::DIAG_MODE:
    switch (diag_current)
    {
    case DIAG::IN:
      // read_input_switches();
      lcd.setCursor(0, 0);
      lcd.print(F("IG"));
      lcd.print(status.ignition);

      lcd.setCursor(4, 0);
      lcd.print(F("AT"));
      lcd.print(status.auto_trans);

      lcd.setCursor(8, 0);
      lcd.print(F("TM"));
      lcd.print(status.test_mode);

      lcd.setCursor(12, 0);
      lcd.print(F("RM"));
      lcd.print(status.read_mode);

      lcd.setCursor(0, 1);
      lcd.print(F("NT"));
      lcd.print(status.neutral);

      lcd.setCursor(4, 1);
      lcd.print(F("PK"));
      lcd.print(status.park);

      lcd.setCursor(8, 1);
      lcd.print(F("CA"));
      lcd.print(status.california);
      break;

    case DIAG::IO:
      // read_io_switches();
      lcd.setCursor(0, 0);
      lcd.print(F("ID"));
      lcd.print(status0.idle_sw);

      lcd.setCursor(4, 0);
      lcd.print(F("AC"));
      lcd.print(status0.ac_sw);

      lcd.setCursor(8, 0);
      lcd.print(F("AR"));
      lcd.print(status0.ac_relay);

      lcd.setCursor(12, 0);
      lcd.print(F("RF"));
      lcd.print(status0.rad_fan);

      lcd.setCursor(0, 1);
      lcd.print(F("FP"));
      lcd.print(status0.fuel_pump);

      lcd.setCursor(4, 1);
      lcd.print(F("CN"));
      lcd.print(status0.purge_valve);

      lcd.setCursor(8, 1);
      lcd.print(F("KS"));
      lcd.print(status0.pinging);

      lcd.setCursor(12, 1);
      lcd.print(F("PX"));
      lcd.print(status0.press_exch);
      break;

    case DIAG::AC_ONE:
      // read_active_trouble_code_one();
      lcd.setCursor(0, 0);
      lcd.print((uint8_t)11);
      lcd.print(status1.crank);

      lcd.setCursor(4, 0);
      lcd.print((uint8_t)12);
      lcd.print(status1.starter);

      lcd.setCursor(8, 0);
      lcd.print((uint8_t)13);
      lcd.print(status1.cam);

      lcd.setCursor(12, 0);
      lcd.print((uint8_t)14);
      lcd.print(status1.inj_1);

      lcd.setCursor(0, 1);
      lcd.print((uint8_t)15);
      lcd.print(status1.inj_2);

      lcd.setCursor(4, 1);
      lcd.print((uint8_t)16);
      lcd.print(status1.inj_3);

      lcd.setCursor(8, 1);
      lcd.print((uint8_t)17);
      lcd.print(status1.inj_4);
      break;

    case DIAG::AC_TWO:
      // read_active_trouble_code_two();
      lcd.setCursor(0, 0);
      lcd.print((uint8_t)18);
      lcd.print(status2.temp);

      lcd.setCursor(4, 0);
      lcd.print((uint8_t)22);
      lcd.print(status2.knock);

      lcd.setCursor(8, 0);
      lcd.print((uint8_t)23);
      lcd.print(status2.maf);

      lcd.setCursor(12, 0);
      lcd.print((uint8_t)45);
      lcd.print(status2.iacv);

      lcd.setCursor(0, 1);
      lcd.print((uint8_t)31);
      lcd.print(status2.tps);

      lcd.setCursor(4, 1);
      lcd.print((uint8_t)32);
      lcd.print(status2.oxygen);

      lcd.setCursor(8, 1);
      lcd.print((uint8_t)33);
      lcd.print(status2.vss);

      lcd.setCursor(12, 1);
      lcd.print((uint8_t)35);
      lcd.print(status2.purge);
      break;

    case DIAG::AC_THREE:
      // read_active_trouble_code_three();
      lcd.setCursor(0, 0);
      lcd.print((uint8_t)41);
      lcd.print(status3.fuel_trim);

      lcd.setCursor(4, 0);
      lcd.print((uint8_t)42);
      lcd.print(status3.idle_sw);

      lcd.setCursor(8, 0);
      lcd.print((uint8_t)44);
      lcd.print(status3.wgc);

      lcd.setCursor(12, 0);
      lcd.print((uint8_t)45);
      lcd.print(status3.baro);

      lcd.setCursor(0, 1);
      lcd.print((uint8_t)49);
      lcd.print(status3.wrong_maf);

      lcd.setCursor(4, 1);
      lcd.print((uint8_t)51);
      lcd.print(status3.neutral_sw);

      lcd.setCursor(8, 1);
      lcd.print((uint8_t)52);
      lcd.print(status3.parking_sw);
      break;

    case DIAG::SC_ONE:
      // read_stored_trouble_code_one();
      lcd.setCursor(0, 0);
      lcd.print((uint8_t)11);
      lcd.print(status1.crank);

      lcd.setCursor(4, 0);
      lcd.print((uint8_t)12);
      lcd.print(status1.starter);

      lcd.setCursor(8, 0);
      lcd.print((uint8_t)13);
      lcd.print(status1.cam);

      lcd.setCursor(12, 0);
      lcd.print((uint8_t)14);
      lcd.print(status1.inj_1);

      lcd.setCursor(0, 1);
      lcd.print((uint8_t)15);
      lcd.print(status1.inj_2);

      lcd.setCursor(4, 1);
      lcd.print((uint8_t)16);
      lcd.print(status1.inj_3);

      lcd.setCursor(8, 1);
      lcd.print((uint8_t)17);
      lcd.print(status1.inj_4);
      break;

    case DIAG::SC_TWO:
      // read_stored_trouble_code_two();
      lcd.setCursor(0, 0);
      lcd.print((uint8_t)21);
      lcd.print(status2.temp);

      lcd.setCursor(4, 0);
      lcd.print((uint8_t)22);
      lcd.print(status2.knock);

      lcd.setCursor(8, 0);
      lcd.print((uint8_t)23);
      lcd.print(status2.maf);

      lcd.setCursor(12, 0);
      lcd.print((uint8_t)45);
      lcd.print(status2.iacv);

      lcd.setCursor(0, 1);
      lcd.print((uint8_t)31);
      lcd.print(status2.tps);

      lcd.setCursor(4, 1);
      lcd.print((uint8_t)32);
      lcd.print(status2.oxygen);

      lcd.setCursor(8, 1);
      lcd.print((uint8_t)33);
      lcd.print(status2.vss);

      lcd.setCursor(12, 1);
      lcd.print((uint8_t)35);
      lcd.print(status2.purge);
      break;

    case DIAG::SC_THREE:
      // read_stored_trouble_code_three();
      lcd.setCursor(0, 0);
      lcd.print((uint8_t)41);
      lcd.print(status3.fuel_trim);

      lcd.setCursor(4, 0);
      lcd.print((uint8_t)42);
      lcd.print(status3.idle_sw);

      lcd.setCursor(8, 0);
      lcd.print((uint8_t)44);
      lcd.print(status3.wgc);

      lcd.setCursor(12, 0);
      lcd.print((uint8_t)45);
      lcd.print(status3.baro);

      lcd.setCursor(0, 1);
      lcd.print((uint8_t)49);
      lcd.print(status3.wrong_maf);

      lcd.setCursor(4, 1);
      lcd.print((uint8_t)51);
      lcd.print(status3.neutral_sw);

      lcd.setCursor(8, 1);
      lcd.print((uint8_t)52);
      lcd.print(status3.parking_sw);
      break;

    case DIAG::CLEAR:

      lcd.setCursor(2, 0);
      lcd.print("CLEAR CODES?");

      btn2 = digitalRead(BUTTON2_PIN);
      if (btn2 == LOW && btn2_prev == HIGH)
      {
        PORTB |= (1 << PB7);
        lcd.clear();
        lcd.print("CLEARING........");
        _delay_ms(100);
        lcd.clear();
      }
      break;

    default:
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

    default:
      break;
    }
    break;

  case MENU::PARAMS_BIG:
    switch (big_current)
    {
    case BIG::BIG_ONE:
      break;

    case BIG::BIG_TWO:
      break;

    case BIG::BIG_THREE:
      break;

    case BIG::BIG_FOUR:
      break;

    case BIG::BIG_FIVE:
      break;

    case BIG::BIG_SIX:
      break;

    case BIG::BIG_SEV:
      break;

    case BIG::BIG_EIGHT:
      break;

    default:
      break;
    }
    break;

  default:
    break;
  }
}
#pragma GCC diagnostic pop

/*
if (USBSerial.available())
{
  char cmd[5] = {0};
  USBSerial.readBytes(cmd, 5);
  // USBSerial.clear();

  if (!strncmp(cmd, "ROAD ", 5))
  { // Road Mode
    driving_mode = DRIVING_MODE_NORMAL;
    USBSerial.println("DEBUG: Sport mode off");
  }

  if (!strncmp(cmd, "SPORT", 5))
  { // Sport Mode
    driving_mode = DRIVING_MODE_SPORT;
    USBSerial.println("DEBUG: Sport mode on");
  }

  if (!strncmp(cmd, "SPEED", 5))
  { // Speed limiter tests
    driving_mode = DRIVING_MODE_SPEED_ONLY;
    USBSerial.println("DEBUG: Speed mode on");
  }

  // Reset timer in case of change
  increment = 0;
}

if (driving_mode == DRIVING_MODE_SPEED_ONLY)
{
  speed = read_speed();
  rpm = DATA_NOT_AVAILABLE_CONSTANT;
  throttle = DATA_NOT_AVAILABLE_CONSTANT;
  coolant_temp = DATA_NOT_AVAILABLE_CONSTANT;
}
else
{
  if (increment % LEVEL_1_REFRESH == 0)
  {
    speed = read_speed();
    rpm = read_rpm();
    throttle = read_throttle_percentage();
  }

  if (driving_mode == DRIVING_MODE_NORMAL)
  {
    if (increment % LEVEL_2_REFRESH == 0)
    {
      airflow = read_airflow();
      manifold_pressure = read_manifold_pressure();
    }
    if (increment % LEVEL_3_REFRESH == 0)
    {
    }
    if (increment % LEVEL_4_REFRESH == 0)
    {
    }
    if (increment % LEVEL_5_REFRESH == 0)
    {
      battery_voltage = read_battery_voltage();
    }
  }

  if (increment % LEVEL_6_REFRESH == 0)
  {
    coolant_temp = read_coolant_temp();
  }
}
#if PRINT_DEBUG_MESSAGES_ON_USB
USBSerial.println("SPE - RPM - TPS - ºC - MAF - Volts - MAP");
USBSerial.print(speed);
USBSerial.print(" -  ");
USBSerial.print(rpm);
USBSerial.print(" -  ");
USBSerial.print(throttle);
USBSerial.print(" -  ");
USBSerial.print(coolant_temp);
USBSerial.print(" -  ");
USBSerial.print(airflow);
USBSerial.print(" -  ");
USBSerial.print(manifold_pressure);
USBSerial.print(" -  ");
USBSerial.println(battery_voltage);
USBSerial.print(" -  ");
USBSerial.println(o2_average);
#else
USBSerial.print(speed);
USBSerial.print("-");
USBSerial.print(rpm);
USBSerial.print("-");
USBSerial.print(throttle);
USBSerial.print("-");
USBSerial.print(coolant_temp);
USBSerial.print("-");
USBSerial.print(airflow);
USBSerial.print("-");
USBSerial.print(manifold_pressure);
USBSerial.print("-");
USBSerial.print(battery_voltage);
USBSerial.println("");
#endif
increment++;
UNUSED_VARIABLE(speed);
UNUSED_VARIABLE(rpm);
UNUSED_VARIABLE(throttle);
UNUSED_VARIABLE(coolant_temp);
UNUSED_VARIABLE(manifold_pressure);
UNUSED_VARIABLE(airflow);
UNUSED_VARIABLE(o2_min);
UNUSED_VARIABLE(o2_max);
}
*/