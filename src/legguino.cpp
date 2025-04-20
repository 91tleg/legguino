#include <Arduino.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "state_menu.h"
#include "state_execute.h"
#include "utilities.h"

void setup()
{
    DDRB |= (1 << PB7);
    lcd.init();
    lcd.backlight();
    lbg.begin();
    btn1_init();
    btn2_init();

    HWSerial.begin(1953, SERIAL_8E1);
    /*
    UBRR1 = 0x0672;  // 1953 baud
    UCSR1A &= ~(1 << U2X1);  // Disable double speed mode
    UCSR1C = (1 << UPM11);   // Even parity, 8-bit data
    UCSR1B = (1 << RXEN1) | (1 << TXEN1); // Enable receiver and transmitter
    */

    // HWSerial.begin(9600, SERIAL_8N1); // Python

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

unsigned long prev_millis = 0;
unsigned long interval = 650;  // Delay to avoid sending requests too fast

void __attribute__((always_inline)) loop()
{
    unsigned long current_millis = millis();
    if (current_millis - prev_millis >= interval)
    {
        prev_millis = current_millis;
        execute_state();
        check_return_menu();

    }
}
#pragma GCC diagnostic pop