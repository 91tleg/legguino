#include "state_diag.h"
#include "legguino.h"

DIAG diag_current = DIAG::IN;
input_switches status = {0};
io_switches status0 = {0};
trouble_code_one status1 = {0};
trouble_code_two status2 = {0};
trouble_code_three status3 = {0};

void diag_page_control(void)
{
    if (diag_current > DIAG::CLEAR)
        diag_current = DIAG::IN;
    btn1 = digitalRead(BUTTON1_PIN);
    if (btn1 == LOW && btn1_prev == HIGH)
    {
        PORTB |= (1 << PB7);
        _delay_ms(100);
        PORTB &= ~(1 << PB7);
        diag_current = static_cast<DIAG>(static_cast<uint8_t>(diag_current) + 1);
    }
    btn1_prev = btn1;
}

void lcd_print_input_sw(void)
{
    sprintf(buffer, "IG%d AT%d TM%d RM%d", status.ignition, status.auto_trans, status.test_mode, status.read_mode);
    lcd.setCursor(0, 0);
    lcd.print(buffer);
    sprintf(buffer, "NT%d PK%d CA%d     ", status.neutral, status.park, status.california);
    lcd.setCursor(0, 1);
    lcd.print(buffer);
}

void lcd_print_io_sw(void)
{
    sprintf(buffer, "ID%d AC%d AR%d RF%d", status0.idle_sw, status0.ac_sw, status0.ac_relay, status0.rad_fan);
    lcd.setCursor(0, 0);
    lcd.print(buffer);
    sprintf(buffer, "FP%d CN%d KS%d PX%d", status0.fuel_pump, status0.purge_valve, status0.pinging, status0.press_exch);
    lcd.setCursor(0, 1);
    lcd.print(buffer);
}

void lcd_print_code_one(void)
{
    sprintf(buffer, "11%d 12%d 13%d 14%d", status1.crank, status1.starter, status1.cam, status1.inj_1);
    lcd.setCursor(0, 0);
    lcd.print(buffer);
    sprintf(buffer, "15%d 16%d 17%d    ", status1.inj_2, status1.inj_3, status1.inj_4);
    lcd.setCursor(0, 1);
    lcd.print(buffer);
}

void lcd_print_code_two(void)
{
    sprintf(buffer, "21%d 22%d 23%d 24%d", status2.temp, status2.knock, status2.maf, status2.iacv);
    lcd.setCursor(0, 0);
    lcd.print(buffer);
    sprintf(buffer, "31%d 32%d 33%d 35%d", status2.tps, status2.oxygen, status2.vss, status2.purge);
    lcd.setCursor(0, 1);
    lcd.print(buffer);
}

void lcd_print_code_three(void)
{
    sprintf(buffer, "41%d 42%d 44%d 45%d", status3.fuel_trim, status3.idle_sw, status3.wgc, status3.baro);
    lcd.setCursor(0, 0);
    lcd.print(buffer);
    sprintf(buffer, "49%d 51%d 52%d    ", status3.wrong_maf, status3.neutral_sw, status3.parking_sw);
    lcd.setCursor(0, 1);
    lcd.print(buffer);
}

void check_clear_code(void)
{
    btn2 = digitalRead(BUTTON2_PIN);
    if (btn2 == LOW && btn2_prev == HIGH)
    {
        lcd.setCursor(0, 0);
        lcd.print("CLEARING........");
        send_clear_command(ACTIVE_TROUBLE_CODE_ONE_ADDR);
        send_clear_command(ACTIVE_TROUBLE_CODE_TWO_ADDR);
        send_clear_command(ACTIVE_TROUBLE_CODE_THREE_ADDR);
        _delay_ms(1000);
        send_clear_command(STORED_TROUBLE_CODE_ONE_ADDR);
        send_clear_command(STORED_TROUBLE_CODE_TWO_ADDR);
        send_clear_command(STORED_TROUBLE_CODE_THREE_ADDR);
        lcd.clear();
        lcd_current_page = static_cast<SCAN>(static_cast<uint8_t>(lcd_current_page) + 1);
    }
    _delay_ms(100);
    btn2_prev = digitalRead(BUTTON2_PIN);
}