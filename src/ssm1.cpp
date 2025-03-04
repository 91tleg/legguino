#include "ssm1.h"

void stop_read(void)
{
    uint8_t txbuf[4] = {0x12, 0x00, 0x00, 0x00};
    for (uint8_t i = 0; i < 4; ++i)
    {
        HWSerial.write(txbuf[i]);
    }
    HWSerial.flush();
    _delay_ms(50);
}

void get_romid(uint8_t *buffer)
{
    uint8_t read_command[4] = {0x78, 0x00, 0x00, 0x00};
    uint8_t romid_command[4] = {0x00, 0x46, 0x48, 0x49};
    uint8_t romid[3] = {0};
    for (uint8_t i = 0; i < 4; ++i)
    {
        HWSerial.write(read_command[i]);
    }
    HWSerial.flush();
    while (HWSerial.available())
        HWSerial.read(); // Flush buffer
    for (uint8_t i = 0; i < 4; ++i)
    {
        HWSerial.write(romid_command[i]);
    }
    HWSerial.flush();
    _delay_ms(250);

    int bytes_read = 0;
    while (bytes_read < 3)
    {
        if (HWSerial.available())
        {
            romid[bytes_read] = HWSerial.read();
            ++bytes_read;
        }
    }
    for (uint8_t i = 0; i < 3; ++i)
    {
        buffer[i] = romid[i]; // Copy received ROM ID into buffer
    }
    stop_read();
}

uint8_t read_data_from_address(uint16_t addr)
{
    uint8_t read_command[4] = {0x78, static_cast<uint8_t>(addr >> 8), static_cast<uint8_t>(addr & 0xff), 0x00};
    uint8_t answer[3] = {0};
    //stop_read();
    while (HWSerial.available())
        HWSerial.read(); // Flush the buffer
    for (uint8_t i = 0; i < 4; ++i)
    {
       HWSerial.write(read_command[i]);
    }
    HWSerial.flush();
    _delay_ms(250);
    int retries = 0;
    while (retries < 3)
    {
        if (HWSerial.available() >= 3) // Ensure 3 bytes are available before reading
        {
            int nbytes = HWSerial.readBytes(answer, 3);
            if ((nbytes == 3) && (answer[0] == read_command[1]) && (answer[1] == read_command[2]))
                break; // Valid response received
        }
        ++retries;
    }
    stop_read();
    return answer[2]; // Return the data byte
}

void read_battery_voltage(void)
{
    ecu_parameters.vb = static_cast<float>(read_data_from_address(BATTERY_VOLTAGE_ADDR)) * 0.08;
}

void read_speed(void)
{
    ecu_parameters.vsp = read_data_from_address(SPEED_ADDR) * 16 / 10;
}

void read_rpm(void)
{
    ecu_parameters.erev = static_cast<uint16_t>(read_data_from_address(RPM_ADDR) * 25);
}

void read_coolant_temp(void)
{   
    uint8_t index = read_data_from_address(COOLANT_ADDR);
    int16_t value = static_cast<int16_t>((pgm_read_byte(&coolant_look_up_table[index])));
    if (index < 14)
        value += 255;
    else if (index >= 256 - 29)
        value = -value;
    ecu_parameters.tw = value;
}

void read_airflow(void)
{
    ecu_parameters.qa = static_cast<float>(read_data_from_address(AIRFLOW_SENSOR_ADDR)) / 50;
}

void read_throttle_percentage(void)
{
    ecu_parameters.tps = read_data_from_address(THROTTLE_ADDR) * 100 / 256;
}

void read_throttle_signal(void)
{
    ecu_parameters.thv = static_cast<float>(read_data_from_address(THROTTLE_ADDR) * 100) / 256;
}

void read_manifold_pressure(void)
{
    ecu_parameters.manip = static_cast<float>(read_data_from_address(MANIFOLD_PRESSURE_ADDR)) / 0.128 - 1060;
}

void read_boost_control_duty_cycle(void)
{
    ecu_parameters.wgc = static_cast<float>(read_data_from_address(BOOST_SOLENOID_ADDR) * 100) / 256;
}
void read_ignition_timing(void)
{
    ecu_parameters.advs = read_data_from_address(IGNITION_ADVANCE_ADDR);
}

void read_load(void)
{
    ecu_parameters.ldata = read_data_from_address(ENGINE_LOAD_ADDR);
}

void read_injector_pulse_width(void)
{
    ecu_parameters.tim = static_cast<float>(read_data_from_address(INJECTOR_PULSE_WIDTH_ADDR)) * 2.048;
}

void read_iacv_duty_cycle(void)
{
    ecu_parameters.isc = read_data_from_address(ISU_DUTY_VALVE_ADDR) / 2;
}

void read_o2_signal(void)
{
    ecu_parameters.o2r = static_cast<float>(read_data_from_address(O2_AVERAGE_ADDR)) / 100;
}

void read_timing_correction(void)
{
    ecu_parameters.rtrd = read_data_from_address(TIMING_CORRECTION_ADDR);
}

void read_fuel_trim(void)
{
    ecu_parameters.alphar = static_cast<float>(read_data_from_address(AF_CORRECTION_ADDR) - 128) / 1.28;
}

void read_atmosphere_pressure(void)
{
    ecu_parameters.barop = static_cast<float>(930 - read_data_from_address(ATMOSPHERIC_PRESSURE_ADDR)) * 3.09;
}

void read_input_switches(void)
{
    uint8_t value = read_data_from_address(INPUT_SWITCHES_ADDR);
    status.ignition = (value & (1 << 7)) ? 1 : 0;
    status.auto_trans = (value & (1 << 6)) ? 1 : 0;
    status.test_mode = (value & (1 << 5)) ? 1 : 0;
    status.read_mode = (value & (1 << 4)) ? 1 : 0;
    status.neutral = (value & (1 << 2)) ? 1 : 0;
    status.park = (value & (1 << 1)) ? 1 : 0;
    status.california = (value & (1 << 0)) ? 1 : 0;
}

void read_io_switches(void)
{
    uint8_t value = read_data_from_address(IO_SWITCHES_ADDR);
    status0.idle_sw = (value & (1 << 7)) ? 1 : 0;
    status0.ac_sw = (value & (1 << 6)) ? 1 : 0;
    status0.ac_relay = (value & (1 << 5)) ? 1 : 0;
    status0.rad_fan = (value & (1 << 4)) ? 1 : 0;
    status0.fuel_pump = (value & (1 << 3)) ? 1 : 0;
    status0.purge_valve = (value & (1 << 2)) ? 1 : 0;
    status0.pinging = (value & (1 << 1)) ? 1 : 0;
    status0.press_exch = (value & (1 << 0)) ? 1 : 0;
}

void read_trouble_code_one(uint16_t addr)
{
    uint8_t value = read_data_from_address(addr);
    status1.crank = (value & (1 << 7)) ? 1 : 0;
    status1.starter = (value & (1 << 6)) ? 1 : 0;
    status1.cam = (value & (1 << 5)) ? 1 : 0;
    status1.inj_1 = (value & (1 << 4)) ? 1 : 0;
    status1.inj_2 = (value & (1 << 3)) ? 1 : 0;
    status1.inj_3 = (value & (1 << 2)) ? 1 : 0;
    status1.inj_4 = (value & (1 << 1)) ? 1 : 0;
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
    status2.temp = (value & (1 << 7)) ? 1 : 0;
    status2.knock = (value & (1 << 6)) ? 1 : 0;
    status2.maf = (value & (1 << 5)) ? 1 : 0;
    status2.iacv = (value & (1 << 4)) ? 1 : 0;
    status2.tps = (value & (1 << 3)) ? 1 : 0;
    status2.oxygen = (value & (1 << 2)) ? 1 : 0;
    status2.vss = (value & (1 << 1)) ? 1 : 0;
    status2.purge = (value & (1 << 0)) ? 1 : 0;
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
    status3.fuel_trim = (value & (1 << 7)) ? 1 : 0;
    status3.idle_sw = (value & (1 << 6)) ? 1 : 0;
    status3.wgc = (value & (1 << 4)) ? 1 : 0;
    status3.baro = (value & (1 << 3)) ? 1 : 0;
    status3.wrong_maf = (value & (1 << 2)) ? 1 : 0;
    status3.neutral_sw = (value & (1 << 1)) ? 1 : 0;
    status3.parking_sw = (value & (1 << 0)) ? 1 : 0;
}

void read_active_trouble_code_three(void)
{
    read_trouble_code_three(ACTIVE_TROUBLE_CODE_THREE_ADDR);
}

void read_stored_trouble_code_three(void)
{
    read_trouble_code_three(STORED_TROUBLE_CODE_THREE_ADDR);
}

void send_clear_command(uint16_t addr)
{
    uint8_t clear_command[4] = {0xAA, static_cast<uint8_t>(addr >> 8), static_cast<uint8_t>(addr & 0xff), 0x00};
    for (uint8_t i = 0; i < 4; ++i)
    {
        HWSerial.write(clear_command[i]);
    }
    HWSerial.flush();
}