#include "ssm1.h"

void stop_read(void)
{
    uint8_t txbuf[4] = {0x12, 0x00, 0x00, 0x00};
    for (uint8_t i = 0; i < 4; ++i)
    {
        HWSerial.write(txbuf[i]);
    }
    HWSerial.flush();
    _delay_ms(1000);
}

void display_romid(void)
{
    for (uint8_t i = 0; i < 3; ++i)
    {
        if (ecu_parameters.romid_param[i] < 0x10)
        {
            lcd.print(0); // Print leading zero for single-digit hex values
        }
        lcd.print(ecu_parameters.romid_param[i], HEX);
        if (i < 2)
        {
            lcd.write(static_cast<uint8_t>('.'));
        }
    }
}

bool get_romid(uint8_t *buffer)
{
    uint8_t read_command[4] = {0x78, 0x00, 0x00, 0x00};
    uint8_t romid_command[4] = {0x00, 0x46, 0x48, 0x49};
    uint8_t romid[3] = {0};
    stop_read();

    // HWSerial.clear();
    for (uint8_t i = 0; i < 4; ++i)
    {
        HWSerial.write(read_command[i]);
    }
    _delay_ms(1000);
    HWSerial.flush();
    read_until_no_more();

    int retries = 0;
    while (retries < 3)
    {
        read_until_no_more();
        int nbytes = HWSerial.readBytes(romid, 3);
        if ((nbytes == 3) && (romid[0] != 0x00))
            break;

        // Ask for ROM ID
        for (uint8_t i = 0; i < 4; ++i)
        {
            HWSerial.write(romid_command[i]);
        }
        _delay_ms(1000);
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
    uint8_t romid[3] = {0};
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

uint8_t read_data_from_address_ex(short addr, bool read_once_only)
{
    // HWSerial.clear();
    uint8_t read_command[4] = {0x78, uint8_t(addr >> 8), uint8_t(addr & 0xff), 0x00};
    uint8_t answer[3] = {0};

    for (uint8_t i = 0; i < 4; ++i)
    {
        HWSerial.write(read_command[i]);
    }
    HWSerial.flush();

    if (!read_once_only && HWSerial.available())
    {
        // TODO: test
        HWSerial.readBytes(answer, 3);
        digitalWrite(LED_BUILTIN, HIGH);
        _delay_ms(1000);
        digitalWrite(LED_BUILTIN, LOW);
        _delay_ms(1000);
        return answer[2];
    }

    uint8_t tries = 3;
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
                tries = 3;
            }
        }
        HWSerial.readBytes(answer, 3);
        digitalWrite(LED_BUILTIN, HIGH);
        _delay_ms(1000);
        digitalWrite(LED_BUILTIN, LOW);
        _delay_ms(1000);
    }

    if (read_once_only)
    {
        stop_read();
    }
    return answer[2];
}

static inline uint8_t read_data_from_address(short addr)
{
    return read_data_from_address_ex(addr, true);
}

static inline void read_battery_voltage(void)
{
    ecu_parameters.vb = static_cast<float>(read_data_from_address(BATTERY_VOLTAGE_ADDR)) * 0.08;
}

static inline void read_speed(void)
{
    ecu_parameters.vsp = read_data_from_address(SPEED_ADDR) * 16 / 10;
}

static inline void read_rpm(void)
{
    ecu_parameters.erev = static_cast<uint16_t>(read_data_from_address(RPM_ADDR) * 25);
}

static inline void read_coolant_temp(void)
{
    int16_t value = static_cast<int16_t>(pgm_read_byte(&coolant_look_up_table[read_data_from_address(COOLANT_ADDR)]));
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
    ecu_parameters.qa = static_cast<float>(read_data_from_address(AIRFLOW_SENSOR_ADDR)) / 50;
}

static inline void read_throttle_percentage(void)
{
    ecu_parameters.tps = read_data_from_address(THROTTLE_ADDR);
}

static inline void read_throttle_signal(void)
{
    ecu_parameters.thv = static_cast<float>(read_data_from_address(THROTTLE_ADDR) * 100) / 256;
}

static inline void read_manifold_pressure(void)
{
    ecu_parameters.manip = static_cast<float>(read_data_from_address(MANIFOLD_PRESSURE_ADDR)) / 0.128 - 1060;
}

static inline void read_boost_control_duty_cycle(void)
{
    ecu_parameters.wgc = static_cast<float>(read_data_from_address(BOOST_SOLENOID_ADDR) * 100) / 256;
}

static inline void read_ignition_timing(void)
{
    ecu_parameters.advs = read_data_from_address(IGNITION_ADVANCE_ADDR);
}

static inline void read_load(void)
{
    ecu_parameters.ldata = read_data_from_address(ENGINE_LOAD_ADDR);
}

static inline void read_injector_pulse_width(void)
{
    ecu_parameters.tim = static_cast<float>(read_data_from_address(INJECTOR_PULSE_WIDTH_ADDR)) * 2.048;
}

static inline void read_iacv_duty_cycle(void)
{
    ecu_parameters.isc = read_data_from_address(ISU_DUTY_VALVE_ADDR) / 2;
}

static inline void read_o2_signal(void)
{
    ecu_parameters.o2r = static_cast<float>(read_data_from_address(O2_AVERAGE_ADDR)) / 100;
}

static inline void read_timing_correction(void)
{
    ecu_parameters.rtrd = read_data_from_address(TIMING_CORRECTION_ADDR);
}

static inline void read_fuel_trim(void)
{
    ecu_parameters.alphar = static_cast<float>(read_data_from_address(AF_CORRECTION_ADDR) - 128) / 1.28;
}

static inline void read_atmosphere_pressure(void)
{
    ecu_parameters.barop = static_cast<float>(930 - read_data_from_address(ATMOSPHERIC_PRESSURE_ADDR)) * 3.09;
}

void read_input_switches(void)
{
    uint8_t value = read_data_from_address(INPUT_SWITCHES_ADDR);
    if (value & (1 << 7))
        status.ignition = 1;
    if (value & (1 << 6))
        status.auto_trans = 1;
    if (value & (1 << 5))
        status.test_mode = 1;
    if (value & (1 << 4))
        status.read_mode = 1;
    if (value & (1 << 2))
        status.neutral = 1;
    if (value & (1 << 1))
        status.park = 1;
    if (value & (1 << 0))
        status.california = 1;
}

void read_io_switches(void)
{
    uint8_t value = read_data_from_address(IO_SWITCHES_ADDR);
    if (value & (1 << 7))
        status0.idle_sw = 1;
    if (value & (1 << 6))
        status0.ac_sw = 1;
    if (value & (1 << 5))
        status0.ac_relay = 1;
    if (value & (1 << 4))
        status0.rad_fan = 1;
    if (value & (1 << 3))
        status0.fuel_pump = 1;
    if (value & (1 << 2))
        status0.purge_valve = 1;
    if (value & (1 << 1))
        status0.pinging = 1;
    if (value & (1 << 0))
        status0.press_exch = 1;
}

void read_trouble_code_one(uint16_t addr)
{
    uint8_t value = read_data_from_address(addr);
    if (value & (1 << 7))
        status1.crank = 1;
    if (value & (1 << 6))
        status1.starter = 1;
    if (value & (1 << 5))
        status1.cam = 1;
    if (value & (1 << 4))
        status1.inj_1 = 1;
    if (value & (1 << 3))
        status1.inj_2 = 1;
    if (value & (1 << 2))
        status1.inj_3 = 1;
    if (value & (1 << 1))
        status1.inj_4 = 1;
}

void read_active_trouble_code_one(void)
{
    read_trouble_code_one(ACTIVE_TROUBLE_CODE_ONE_ADDR);
}

void read_stored_trouble_code_one(void)
{
    read_trouble_code_one(STORED_TROUBLE_CODE_THREE_ADDR);
}

void read_trouble_code_two(uint16_t addr)
{
    uint8_t value = read_data_from_address(addr);
    if (value & (1 << 7))
        status2.temp = 1;
    if (value & (1 << 6))
        status2.knock = 1;
    if (value & (1 << 5))
        status2.maf = 1;
    if (value & (1 << 4))
        status2.iacv = 1;
    if (value & (1 << 3))
        status2.tps = 1;
    if (value & (1 << 2))
        status2.oxygen = 1;
    if (value & (1 << 1))
        status2.vss = 1;
    if (value & (1 << 0))
        status2.purge = 1;
}

void read_active_trouble_code_two(void)
{
    read_trouble_code_two(ACTIVE_TROUBLE_CODE_TWO_ADDR);
}

void read_stored_trouble_code_two(void)
{
    read_trouble_code_two(STORED_TROUBLE_CODE_TWO_ADDR);
}

void read_trouble_code_three(uint16_t addr)
{
    uint8_t value = read_data_from_address(addr);
    if (value & (1 << 7))
        status3.fuel_trim = 1;
    if (value & (1 << 6))
        status3.idle_sw = 1;
    if (value & (1 << 4))
        status3.wgc = 1;
    if (value & (1 << 3))
        status3.baro = 1;
    if (value & (1 << 2))
        status3.wrong_maf = 1;
    if (value & (1 << 1))
        status3.neutral_sw = 1;
    if (value & (1 << 0))
        status3.parking_sw = 1;
}

void read_active_trouble_code_three(void)
{
    read_trouble_code_three(ACTIVE_TROUBLE_CODE_THREE_ADDR);
}

void read_stored_trouble_code_three(void)
{
    read_trouble_code_three(STORED_TROUBLE_CODE_THREE_ADDR);
}

// AA MSB LSB 00   (Clear to Zero)
// AA MSB LSB FF   (If inverted value is used)
void send_clear_command(uint16_t addr_high, uint16_t addr_low)
{
    Serial.write(0xAA);
    Serial.write(addr_high); // High byte of address
    Serial.write(addr_low);  // Low byte of address
    Serial.write(0x00);      // Clear value
}