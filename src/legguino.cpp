#include "legguino.h"

void setup()
{
    DDRB |= (1 << PB7);
    lcd.init();
    lcd.backlight();
    lbg.begin();
    btn1_init();
    btn2_init();

    // HWSerial.begin(1953, SERIAL_8E1);
    /*
    UBRR1 = 0x0672;  // 1953 baud
    UCSR1A &= ~(1 << U2X1);  // Disable double speed mode
    UCSR1C = (1 << UPM11);   // Even parity, 8-bit data
    UCSR1B = (1 << RXEN1) | (1 << TXEN1); // Enable receiver and transmitter
    */

    HWSerial.begin(9600, SERIAL_8N1); // Python

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

void __attribute__((always_inline)) loop()
{
    check_return_menu();
    execute_state();
   //_delay_ms(100); 
   delay_millis(100);  // Delay to avoid sending requests too fast
}
#pragma GCC diagnostic pop