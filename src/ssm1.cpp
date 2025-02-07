#include "ssm1.h"

// Initialize all to 0
ecu_params ecu_parameters = {0};
input_switches status = {0};
io_switches status0 = {0};
trouble_code_one status1 = {0};
trouble_code_two status2 = {0};
trouble_code_three status3 = {0};

void stop_read(void)
{
    byte txbuf[4] = {0x12, 0x00, 0x00, 0x00};
    for (uint8_t i = 0; i < 4; ++i)
    {
        HWSerial.write(txbuf[i]);
    }
    HWSerial.flush();
    for(uint32_t i = 0; i < 4000000; ++i) { __asm__ volatile("nop"); }
}

bool get_romid(byte *buffer)
{
    byte read_command[4] = {0x78, 0x00, 0x00, 0x00};
    byte romid_command[4] = {0x00, 0x46, 0x48, 0x49};
    byte romid[3] = {0};
    stop_read();

    // HWSerial.clear();
    for (uint8_t i = 0; i < 4; ++i)
    {
        HWSerial.write(read_command[i]);
    }
    for(uint32_t i = 0; i < 4000000; ++i) { __asm__ volatile("nop"); }
    HWSerial.flush();
    read_until_no_more();

    int retries = 0;
    while (retries < NUM_READ_TRIES)
    {
        read_until_no_more();
        int nbytes = HWSerial.readBytes(romid, 3);

        if ((nbytes == 3) && (romid[0] != 0x00))
        {
            break;
        }

        // Ask for ROM ID
        for (uint8_t i = 0; i < 4; ++i)
        {
            HWSerial.write(romid_command[i]);
        }
        for(uint32_t i = 0; i < 4000000; ++i) { __asm__ volatile("nop"); }
        HWSerial.flush();
        ++retries;

        #if PRINT_DEBUG_MESSAGES_ON_USB
            USBSerial.println("Testing ROM ID once again");
        #endif
    }
    stop_read();

    if (romid[0] == 0x00)
    {
        return false; // ROM ID retrieval failed
    }

    // Copies the retrieved ROM ID into buffer
    for (uint8_t i = 0; i < 3; ++i)
    {
        buffer[i] = romid[i];
    }

    return true;
}

void query_romid(void)
{
    byte romid[3] = {0};
    if (get_romid(romid))
    {

        #if PRINT_DEBUG_MESSAGES_ON_USB
            USBSerial.print("Received ROMID: ");
            USBSerial.print(romid[0], HEX);
            USBSerial.print(romid[1], HEX);
            USBSerial.println(romid[2], HEX);
        #endif
        read_until_no_more();
    }
}

byte read_data_from_address_ex(short addr, bool read_once_only)
{
    // HWSerial.clear();
    byte read_command[4] = {0x78, byte(addr >> 8), byte(addr & 0xff), 0x00};
    byte answer[3] = {0};

    for (uint8_t i = 0; i < 4; ++i)
    {
        HWSerial.write(read_command[i]);
    }
    HWSerial.flush();

    if (!read_once_only && HWSerial.available())
    {
        // TODO: test
        HWSerial.readBytes(answer, 3);
        digitalWrite(LED_PIN, HIGH);
        for (uint32_t i = 0; i < 4000000; ++i) { __asm__ volatile("nop"); }
        digitalWrite(LED_PIN, LOW);
        for (uint32_t i = 0; i < 4000000; ++i) { __asm__ volatile("nop"); }
        return answer[2];
    }

    uint8_t tries = NUM_READ_TRIES;
    while (answer[0] != read_command[1] || answer[1] != read_command[2])
    {
        if (!HWSerial.available())
        {
            --tries;
            if (!tries)
            {
                // HWSerial.clear();

                for (uint8_t i = 0; i < 4; ++i)
                {
                    HWSerial.write(read_command[i]);
                }
                tries = NUM_READ_TRIES;
            }
        }
        HWSerial.readBytes(answer, 3);
        digitalWrite(LED_PIN, HIGH);
        for (uint32_t i = 0; i < 4000000; ++i) { __asm__ volatile("nop"); }
        digitalWrite(LED_PIN, LOW);
        for (uint32_t i = 0; i < 4000000; ++i) { __asm__ volatile("nop"); }
    }

    if (read_once_only)
    {
        stop_read();
    }
    return answer[2];
}

static inline byte read_data_from_address(short addr)
{
    return read_data_from_address_ex(addr, true);
}

static inline void read_battery_voltage(void)
{
    float value = read_data_from_address(BATTERY_VOLTAGE_ADDR) * 0.08;
    ecu_parameters.vb = value;
}

static inline void read_speed(void)
{
    uint8_t value = read_data_from_address(SPEED_ADDR) * 1.6; // 2?
    ecu_parameters.vsp = value;
}

static inline void read_rpm(void)
{
    uint16_t value = read_data_from_address(RPM_ADDR) * 25;
    ecu_parameters.erev = value;
}

static inline void read_coolant_temp(void) 
{
    int16_t value = pgm_read_byte(&coolant_look_up_table[read_data_from_address(COOLANT_ADDR)]);
    if (value < 14) 
    {   
        value += 255;
    }
    if (value > (255 - 29)) 
    {
        value = -value;
    }
    ecu_parameters.tw = value;
}

static inline void read_airflow(void)
{
    uint16_t value = (read_data_from_address(AIRFLOW_SENSOR_ADDR) * 100) / 256;
    ecu_parameters.qa = value;
}

static inline void read_throttle_percentage(void)
{
    uint16_t value = (read_data_from_address(THROTTLE_ADDR) * 100) / 256;
    ecu_parameters.tps = value;
}

static inline void read_throttle_signal(void)
{
    uint16_t value = read_data_from_address(THROTTLE_ADDR);
    ecu_parameters.thv = value;
}

static inline void read_manifold_pressure(void)
{
    int16_t value = (read_data_from_address(MANIFOLD_PRESSURE_ADDR) / 0.128) - 1060;
    ecu_parameters.manip  = value;
}

static inline void read_boost_control_duty_cycle(void)
{
    uint16_t value = (read_data_from_address(BOOST_SOLENOID_ADDR) * 100) / 256;
    ecu_parameters.wgc = value;
}

static inline void read_ignition_timing(void)
{
    uint16_t value = read_data_from_address(IGNITION_ADVANCE_ADDR);
    ecu_parameters.advs = value;
}

static inline void read_load(void)
{
    uint16_t value = read_data_from_address(ENGINE_LOAD_ADDR);
    ecu_parameters.ldata = value;
}

static inline void read_injector_pulse_width(void)
{
    uint16_t value = read_data_from_address(INJECTOR_PULSE_WIDTH_ADDR) * 2.048;
    ecu_parameters.tim = value;
}

static inline void read_iacv_duty_cycle(void)
{
    uint16_t value = read_data_from_address(ISU_DUTY_VALVE_ADDR) / 2;
    ecu_parameters.isc = value;
}

static inline void read_o2_signal(void)
{
    uint16_t value = read_data_from_address(O2_AVERAGE_ADDR) / 100;
    ecu_parameters.o2r = value;
}

static inline void read_timing_correction(void)
{
    uint16_t value = read_data_from_address(KNOCK_CORRECTION_ADDR);
    ecu_parameters.rtrd = value;
}

static inline void read_fuel_trim(void)
{
    uint16_t value = (read_data_from_address(AF_CORRECTION_ADDR) - 128) / 1.28;
    ecu_parameters.alphar = value;
}

// Initial MAP reading taken before the engine is running
static inline void read_atmosphere_pressure(void)
{
    uint16_t value = (930 - read_data_from_address(ATMOSPHERIC_PRESSURE_ADDR)) * 3.09;
    ecu_parameters.barop = value;
}

void read_input_switches(void)
{
    uint16_t value = read_data_from_address(INPUT_SWITCHES_ADDR);
    if (value & (1 << 7))
    {
        status.ignition = 1;
    }
    if (value & (1 << 6))
    {
        status.auto_trans = 1;
    }
    if (value & (1 << 5))
    {
        status.test_mode = 1;
    }
    if (value & (1 << 4))
    {
        status.read_mode = 1;
    }
    if (value & (1 << 2))
    {
        status.neutral = 1;
    }
    if (value & (1 << 1))
    {
        status.park = 1;
    }
    if (value & (1 << 0))
    {
        status.california = 1;
    }
}

void read_io_switches(void)
{
    uint16_t value = read_data_from_address(IO_SWITCHES_ADDR);
    if (value & (1 << 7))
    {
        status0.idle_sw = 1;
    }
    if (value & (1 << 6))
    {
        status0.ac_sw = 1;
    }
    if (value & (1 << 5))
    {
        status0.ac_relay = 1;
    }
    if (value & (1 << 4))
    {
        status0.rad_fan = 1;
    }
    if (value & (1 << 3))
    {
        status0.fuel_pump = 1;
    }
    if (value & (1 << 2))
    {
        status0.purge_valve = 1;
    }
    if (value & (1 << 1))
    {
        status0.pinging = 1;
    }
    if (value & (1 << 0)) 
    {
        status0.press_exch = 1;
    }
}

  /*
   * The current codes are stored as bit flags in locations 0x1026 through 0x1029 and the previous codes are stored
   * in locations 0x12B9 through 0x12BC.
   *
   *            Bit7	      Bit6	       Bit5	      Bit4	          Bit3	      Bit2	      Bit1	      Bit0
   * 1026/12B9	Crank(11)	  Starter(12)	 Cam(13)	  TPS(31)	        O2Right(32)	H2OTemp(21)	Knock1(22)	MAF(23)
   * 1027/12BA	Atmos(45)	  Knock2(28)	 Bypass(24) EGRvalve(34)    VSS2(33)	  O2Left(37)	Purge(35)	  AFlearn(41)
   * 1028/12BB	Inj5(18)	  P-switch(52) Inj6(19)	  N-switch(51)    Inj4(17)	  Inj3(16)	  Inj2(15)	  Inj1(14)
   * 1029/12BC	n/a	        n/a	         n/a	      CO Resistor(49)	TqSig(38)	  EGRsys(56)	Crank2(29)	EGRtemp(55)
   *
   * As an example, suppose location 0x1026 contains the value 0x48. That is 01001000 in binary. This indicates
   * errors with the Starter (Code 12) and Right O2 Sensor (Code 32).
   * 
   * 
   * LEG:
   *            Bit7	      Bit6	       Bit5	      Bit4	          Bit3	      Bit2	      Bit1	      Bit0
   * 1026/12B9	Crank(11)	  Starter(12)	 Cam(13)	  TPS(31)	        O2Right(32)	H2OTemp(21)	Knock1(22)	MAF(23)
   * 1027/12BA	Atmos(45)	  n/a       	 IACV(24)   n/a             VSS(33)	    n/a       	Purge(35)	  AFlearn(41)
   * 1028/12BB	n/a     	  P-switch(52) n/a     	  N-switch(51)    Inj4(17)	  Inj3(16)	  Inj2(15)	  Inj1(14)
   * 1029/12BC	n/a	        n/a	         n/a	      WrongMAF(49)   	n/a     	  n/a        	n/a       	n/a
   * 
   */

void read_active_trouble_code_one(void)
{
    uint16_t value = read_data_from_address(ACTIVE_TROUBLE_CODE_ONE_ADDR);
    if (value & (1 << 7))
    {
        status1.crank = 1;
    }
    if (value & (1 << 6))
    {
        status1.starter = 1;
    }
    if (value & (1 << 5))
    {
        status1.cam = 1;
    }
    if (value & (1 << 4))
    {
        status1.inj_1 = 1;
    }
    if (value & (1 << 3))
    {
        status1.inj_2 = 1;
    }
    if (value & (1 << 2))
    {
        status1.inj_3 = 1;
    }
    if (value & (1 << 1))
    {
        status1.inj_4 = 1;
    }
}

void read_active_trouble_code_two(void)
{
    uint16_t value = read_data_from_address(ACTIVE_TROUBLE_CODE_TWO_ADDR);
    if (value & (1 << 7))
    {
        status2.temp = 1;
    }
    if (value & (1 << 6)) 
    {
        status2.knock = 1;
    }
    if (value & (1 << 5))
    {
        status2.maf = 1;
    }
    if (value & (1 << 4)) 
    {
        status2.iacv = 1;
    }
    if (value & (1 << 3))
    {
        status2.tps = 1;
    }
    if (value & (1 << 2)) 
    {
        status2.oxygen = 1;
    }
    if (value & (1 << 1))
    {
        status2.vss = 1;
    }
    if (value & (1 << 0))
    {
        status2.purge = 1;
    }
}

void read_active_trouble_code_three(void)
{
    uint16_t value = read_data_from_address(ACTIVE_TROUBLE_CODE_ONE_ADDR);
    if (value & (1 << 7)) 
    {
        status3.fuel_trim = 1;
    }
    if (value & (1 << 6))
    {
        status3.idle_sw = 1;
    }
    // if (value & (1 << 5)) {}
    if (value & (1 << 4))
    {
        status3.wgc = 1;
    }
    if (value & (1 << 3))
    {
        status3.baro = 1;
    }
    if (value & (1 << 2))
    {
        status3.wrong_maf = 1;
    }
    if (value & (1 << 1))
    {
        status3.neutral_sw = 1;
    }
    if (value & (1 << 0))
    {
        status3.parking_sw = 1;
    }
}

void read_stored_trouble_code_one(void)
{
    uint16_t value = read_data_from_address(STORED_TROUBLE_CODE_THREE_ADDR);
    if (value & (1 << 7))
    {
        status1.crank = 1;
    }
    if (value & (1 << 6))
    {
        status1.starter = 1;
    }
    if (value & (1 << 5))
    {
        status1.cam = 1;
    }
    if (value & (1 << 4))
    {
        status1.inj_1 = 1;
    }
    if (value & (1 << 3))
    {
        status1.inj_2 = 1;
    }
    if (value & (1 << 2))
    {
        status1.inj_3 = 1;
    }
    if (value & (1 << 1))
    {
        status1.inj_4 = 1;
    }
}

void read_stored_trouble_code_two(void)
{
    uint16_t value = read_data_from_address(STORED_TROUBLE_CODE_TWO_ADDR);
    if (value & (1 << 7))
    {
        status2.temp = 1;
    }
    if (value & (1 << 6)) 
    {
        status2.knock = 1;
    }
    if (value & (1 << 5))
    {
        status2.maf = 1;
    }
    if (value & (1 << 4)) 
    {
        status2.iacv = 1;
    }
    if (value & (1 << 3))
    {
        status2.tps = 1;
    }
    if (value & (1 << 2)) 
    {
        status2.oxygen = 1;
    }
    if (value & (1 << 1))
    {
        status2.vss = 1;
    }
    if (value & (1 << 0))
    {
        status2.purge = 1;
    }
}

void read_stored_trouble_code_three(void)
{
    uint16_t value = read_data_from_address(STORED_TROUBLE_CODE_ONE_ADDR);
    if (value & (1 << 7)) 
    {
        status3.fuel_trim = 1;
    }
    if (value & (1 << 6))
    {
        status3.idle_sw = 1;
    }
    if (value & (1 << 4))
    {
        status3.wgc = 1;
    }
    if (value & (1 << 3))
    {
        status3.baro = 1;
    }
    if (value & (1 << 2))
    {
        status3.wrong_maf = 1;
    }
    if (value & (1 << 1))
    {
        status3.neutral_sw = 1;
    }
    if (value & (1 << 0))
    {
        status3.parking_sw = 1;
    }
}

void send_clear_command(uint16_t addr_high, uint16_t addr_low) 
{
    Serial.write(0xAA);
    Serial.write(addr_high);  // High byte of address
    Serial.write(addr_low);  // Low byte of address
    Serial.write(0x00);  // Clear value 
}