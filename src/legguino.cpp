#include "legguino.h"

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

extern uint8_t lcd_current_page;
extern uint8_t button1_state;
extern uint8_t button2_state;
extern ecu_params ecu_parameters;
extern input_switches status;
extern io_switches status0;
extern trouble_code_one status1;
extern trouble_code_two status2;
extern trouble_code_three status3;

void setup()
{
  lcd_init();

  pinMode(LED_PIN, OUTPUT);

  // Serial to read from Subaru, pins 0 and 1
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


  //char cmd[4] = {0};
  //USBSerial.readBytes(cmd, 4);
  // USBSerial.clear();
  //if (!strncmp(cmd, "DUMP", 4))
  //{
  //  dump_address_space(0, 0xffff);
  //}
  byte romid_buffer[3];  // Buffer to store ROM ID
  if (get_romid(romid_buffer)) 
  {  
    uint16_t romid_16bit = ((uint16_t)romid_buffer[0] << 8) | romid_buffer[1];
    ecu_parameters.romid_param = romid_16bit;
  }
  lcd.print(F("ROM ID: "));
  lcd.print(ecu_parameters.romid_param, HEX);
  _delay_ms(5000);
  lcd.clear();

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
/* Sometimes loop() is inlined by LTO & sometimes not
 * When not inlined, there is a huge difference in stack usage: 60+ bytes
 * That eats into available RAM.
 * Adding __attribute__((always_inline)) forces the LTO process to inline.
 * Since the function is declared in an Arduino header, we can't change
 * it to inline, so we need to suppress the resulting warning.
 */
void __attribute__((always_inline)) loop()
{

    // Check if button is pressed
  if (digitalRead(BUTTON1_PIN) == HIGH)
  {
    ++lcd_current_page;
    _delay_ms(1000);
  }
  if (lcd_current_page > LCD_MAX_PAGE_COUNT)
  {
    lcd_current_page = 0;
  }
  if (digitalRead(BUTTON2_PIN) == HIGH)
  {
    --lcd_current_page;
    _delay_ms(1000);
  }
  if (lcd_current_page < 0)
  {
    lcd_current_page = LCD_MAX_PAGE_COUNT;
  }


  switch (lcd_current_page)
  {
  case 0:
    read_battery_voltage();
    lcd.setCursor(0, 0);
    lcd.print(F("VB: "));
    lcd.print(ecu_parameters.vb);

    read_speed();
    lcd.setCursor(8, 0);
    lcd.print(F("SPD:"));
    lcd.print(ecu_parameters.vsp);

    read_rpm();
    lcd.setCursor(0, 1);
    lcd.print(F("REV:"));
    lcd.print(ecu_parameters.erev);

    read_coolant_temp();
    lcd.setCursor(8, 1);
    lcd.print(F("TW: "));
    lcd.print(ecu_parameters.tw);
    _delay_ms(1000);
    lcd.clear();
    break;

  case 1:
    read_fuel_trim();
    lcd.setCursor(0, 0);
    lcd.print(F("ADV:"));
    lcd.print(ecu_parameters.alphar);

    read_airflow();
    lcd.setCursor(8, 0);
    lcd.print(F("MAF:"));
    lcd.print(ecu_parameters.qa);

    read_load();
    lcd.setCursor(0, 1);
    lcd.print(F("LD: "));
    lcd.print(ecu_parameters.ldata);

    read_throttle_percentage();
    lcd.setCursor(8, 1);
    lcd.print(F("TPS:"));
    lcd.print(ecu_parameters.tps);

    _delay_ms(1000);
    lcd.clear();
    break;

  case 2:
    read_injector_pulse_width();
    lcd.setCursor(0, 0);
    lcd.print(F("IPW:"));
    lcd.print(ecu_parameters.tim);

    read_iacv_duty_cycle();
    lcd.setCursor(8, 0);
    lcd.print(F("ISC:"));
    lcd.print(ecu_parameters.isc);

    read_o2_signal();
    lcd.setCursor(0, 1);
    lcd.print(F("O2R:"));
    lcd.print(ecu_parameters.o2r);

    read_timing_correction();
    lcd.setCursor(8, 1);
    lcd.print(F("RTD:"));
    lcd.print(ecu_parameters.advs);

    _delay_ms(1000);
    lcd.clear();
    break;

  case 3:
    read_atmosphere_pressure();
    if (ecu_parameters.romid_param == JECS3_ROMID)  // 1993-1994 JECS non-turbo ECU -- has no barometric pressure reading
    {
      ecu_parameters.barop = 0;
    }
    lcd.setCursor(0, 0);
    lcd.print(F("BAR:"));
    lcd.print(ecu_parameters.barop);

    read_manifold_pressure();
    lcd.setCursor(8, 0);
    lcd.print(F("MP: "));
    lcd.print(ecu_parameters.manip);

    read_boost_control_duty_cycle();
    lcd.setCursor(0, 1);
    lcd.print(F("WGC:"));
    lcd.print(ecu_parameters.wgc);

    read_throttle_signal();
    lcd.setCursor(8, 1);
    lcd.print(F("THV:"));
    lcd.print(ecu_parameters.thv);

    _delay_ms(1000);
    lcd.clear();
    break;

  case 4:
    read_input_switches();
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

    lcd.setCursor(0,1);
    lcd.print(F("NT"));
    lcd.print(status.neutral);

    lcd.setCursor(4,1);
    lcd.print(F("PK"));
    lcd.print(status.park);

    lcd.setCursor(8,1);
    lcd.print(F("CA"));
    lcd.print(status.california);

    _delay_ms(1000);
    lcd.clear();
    break;

  case 5:
    read_io_switches();
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

    lcd.setCursor(0,1);
    lcd.print(F("FP"));
    lcd.print(status0.fuel_pump);

    lcd.setCursor(4,1);
    lcd.print(F("CN"));
    lcd.print(status0.purge_valve);

    lcd.setCursor(8,1);
    lcd.print(F("KS"));
    lcd.print(status0.pinging); 

    lcd.setCursor(12,1);
    lcd.print(F("PX"));
    lcd.print(status0.press_exch);
    
    _delay_ms(1000);
    lcd.clear();
    break;

  case 6:
    read_active_trouble_code_one(); 
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

    _delay_ms(1000);
    lcd.clear();
    break;

  case 7:
    read_active_trouble_code_two();
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

    _delay_ms(1000);
    lcd.clear();
    break;

  case 8:
    read_active_trouble_code_three();
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

    _delay_ms(1000);
    break;

  case 9:
    read_stored_trouble_code_one(); 
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

    _delay_ms(1000);
    lcd.clear();
    break;

  case 10:
    read_stored_trouble_code_two();
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

    _delay_ms(1000);
    lcd.clear();
    break;

  case 11:
    read_stored_trouble_code_three();
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

    _delay_ms(1000);
    lcd.clear();
    break;

  case 12:
    lcd.setCursor(0, 0);
    lcd.println("Clear codes?");
    lcd.println("PRESS BOTH");

    if (digitalRead(BUTTON1_PIN) == HIGH && digitalRead(BUTTON2_PIN) == HIGH)
    {
      lcd.print("Clearing...");
      for (uint8_t i = 0; i <= 3; ++i)
      {
        send_clear_command(ACTIVE_TROUBLE_CODE_THREE_ADDR, ACTIVE_TROUBLE_CODE_THREE_ADDR);
        _delay_ms(1000);
        
      }
      for (uint8_t i = 0; i <= 3; ++i)
      {
        send_clear_command(STORED_TROUBLE_CODE_THREE_ADDR, STORED_TROUBLE_CODE_ONE_ADDR);
        _delay_ms(1000);
      }
      ++lcd_current_page;
    }
    _delay_ms(1000);
    lcd.clear();
    break;

  default:  // Should not be any
    break;
  }
  #pragma GCC diagnostic pop
}

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