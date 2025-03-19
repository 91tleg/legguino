#include "ssm1.h"

void stop_read(void)
{
    uint8_t txbuf[4] = {0x12, 0x00, 0x00, 0x00};
    for (uint8_t i = 0; i < 4; ++i)
    {
        HWSerial.write(txbuf[i]);
    }
    HWSerial.flush();
    _delay_ms(10);
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
    for (uint8_t i = 0; i < 4; ++i)
    {
        HWSerial.write(romid_command[i]);
    }
    HWSerial.flush();
    //_delay_ms(5);

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

uint8_t __attribute__((weak)) read_data_from_address(uint16_t addr)
{
    uint8_t read_command[4] = {0x78, static_cast<uint8_t>(addr >> 8), static_cast<uint8_t>(addr & 0xff), 0x00};
    uint8_t answer[3] = {0};
    for (uint8_t i = 0; i < 4; ++i)
    {
       HWSerial.write(read_command[i]);
    }
    HWSerial.flush();
    _delay_ms(10);
    
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


void send_clear_command(uint16_t addr)
{
    uint8_t clear_command[4] = {0xAA, static_cast<uint8_t>(addr >> 8), static_cast<uint8_t>(addr & 0xff), 0x00};
    for (uint8_t i = 0; i < 4; ++i)
    {
        HWSerial.write(clear_command[i]);
    }
    HWSerial.flush();
}