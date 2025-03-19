#include <unity.h>
#include "parameters.h"
#include "parameter_math.h"

void setUp(void) {}
void tearDown(void) {}

uint8_t mock_data;
uint8_t read_data_from_address(uint16_t addr)
{
    return mock_data;
}

/* =======================================PARAMETERS======================================== */

// Wwhen index < 14 (255 is added)
void test_read_coolant_temp_index_less_than_14(void)
{
    mock_data = 5;
    read_coolant_temp();
    TEST_ASSERT_EQUAL_INT(331, ecu_parameters.tw);
}

// When index >= 256 - 29 (value is negated)
void test_read_coolant_temp_index_greater_than_256_minus_29(void)
{
    mock_data = 255;
    read_coolant_temp();
    TEST_ASSERT_EQUAL_INT(-47, ecu_parameters.tw);
}

// When index is 14 to 227
void test_read_coolant_temp_normal_case(void)
{
    mock_data = 15;
    read_coolant_temp();
    TEST_ASSERT_EQUAL_INT(243, ecu_parameters.tw);
}

void test_battery_voltage(void)
{
    mock_data = 0;
    read_battery_voltage();
    TEST_ASSERT_EQUAL_FLOAT(0, ecu_parameters.vb);

    mock_data = 255;
    read_battery_voltage();
    TEST_ASSERT_EQUAL_FLOAT(20.4, ecu_parameters.vb);
}

void test_speed(void)
{
    mock_data = 0;
    read_speed();
    TEST_ASSERT_EQUAL_INT(0, ecu_parameters.vsp);

    mock_data = 255;
    read_speed();
    TEST_ASSERT_EQUAL_INT(159, ecu_parameters.vsp);
}

void test_rpm(void)
{
    mock_data = 0;
    read_rpm();
    TEST_ASSERT_EQUAL_INT(0, ecu_parameters.erev);

    mock_data = 255;
    read_rpm();
    TEST_ASSERT_EQUAL_INT(6375, ecu_parameters.erev);
}

void test_airflow(void)
{
    mock_data = 0;
    read_airflow();
    TEST_ASSERT_EQUAL_FLOAT(0, ecu_parameters.qa);

    mock_data = 255;
    read_airflow();
    TEST_ASSERT_EQUAL_FLOAT(5.1, ecu_parameters.qa);
}

void test_throttle_percentage(void)
{
    mock_data = 0;
    read_throttle_percentage();
    TEST_ASSERT_EQUAL_INT(0, ecu_parameters.tps);

    mock_data = 255;
    read_throttle_percentage();
    TEST_ASSERT_EQUAL_INT(99, ecu_parameters.tps);
}

void test_throttle_signal(void)
{
    mock_data = 0;
    read_throttle_signal();
    TEST_ASSERT_EQUAL_FLOAT(0, ecu_parameters.thv);

    mock_data = 255;
    read_throttle_signal();
    int throttle_signal = ecu_parameters.thv;
    TEST_ASSERT_EQUAL_INT(99, throttle_signal);
}

void test_mainfold_pressure(void)
{
    mock_data = 0;
    read_manifold_pressure();
    TEST_ASSERT_EQUAL_FLOAT(-1060, ecu_parameters.manip);

    mock_data = 255;
    read_manifold_pressure();
    TEST_ASSERT_EQUAL_FLOAT(932.18, ecu_parameters.manip);
}

void test_boost_control_duty_cycle(void)
{
    mock_data = 0;
    read_boost_control_duty_cycle();
    TEST_ASSERT_EQUAL_FLOAT(0, ecu_parameters.wgc);
}

void test_ignition_timing(void)
{
    mock_data = 0;
    read_ignition_timing();
    TEST_ASSERT_EQUAL_INT(0, ecu_parameters.advs);
}

void test_load(void)
{
    mock_data = 0;
    read_load();
    TEST_ASSERT_EQUAL_INT(0, ecu_parameters.ldata);
}

void test_injector_pulse_width(void)
{
    mock_data = 0;
    read_injector_pulse_width();
    TEST_ASSERT_EQUAL_FLOAT(0, ecu_parameters.tim);
}

void test_iacv_duty_cycle(void)
{
    mock_data = 0;
    read_iacv_duty_cycle();
    TEST_ASSERT_EQUAL_INT(0, ecu_parameters.isc);
}

void test_o2_signal(void)
{
    mock_data = 0;
    read_o2_signal();
    TEST_ASSERT_EQUAL_INT(0, ecu_parameters.o2r);
}

void test_timing_correction(void)
{
    mock_data = 0;
    read_timing_correction();
    TEST_ASSERT_EQUAL_INT(0, ecu_parameters.rtrd);
}

void test_fuel_trim(void)
{
    mock_data = 0;
    read_fuel_trim();
    TEST_ASSERT_EQUAL_FLOAT(-100, ecu_parameters.alphar);
}

void test_atmosphere_pressure(void)
{
    mock_data = 0;
    read_atmosphere_pressure();
    TEST_ASSERT_EQUAL_FLOAT(2873.7, ecu_parameters.barop);
}

/* ====================================BINARY PARAMETERS===================================== */

void test_input_switch(void)
{
    mock_data = 0;
    read_input_switches();
    TEST_ASSERT_EQUAL_INT(0, status.ignition);
    TEST_ASSERT_EQUAL_INT(0, status.auto_trans);
    TEST_ASSERT_EQUAL_INT(0, status.test_mode);
    TEST_ASSERT_EQUAL_INT(0, status.read_mode);
    TEST_ASSERT_EQUAL_INT(0, status.unused_bit);
    TEST_ASSERT_EQUAL_INT(0, status.neutral);
    TEST_ASSERT_EQUAL_INT(0, status.park);
    TEST_ASSERT_EQUAL_INT(0, status.california);

    mock_data = 255;
    read_input_switches();
    TEST_ASSERT_EQUAL_INT(1, status.ignition);
    TEST_ASSERT_EQUAL_INT(1, status.auto_trans);
    TEST_ASSERT_EQUAL_INT(1, status.test_mode);
    TEST_ASSERT_EQUAL_INT(1, status.read_mode);
    TEST_ASSERT_EQUAL_INT(0, status.unused_bit);
    TEST_ASSERT_EQUAL_INT(1, status.neutral);
    TEST_ASSERT_EQUAL_INT(1, status.park);
    TEST_ASSERT_EQUAL_INT(1, status.california);
}

void test_in_out_switch(void)
{
    mock_data = 0;
    read_io_switches();
    TEST_ASSERT_EQUAL_INT(0, status0.idle_sw);
    TEST_ASSERT_EQUAL_INT(0, status0.ac_sw);
    TEST_ASSERT_EQUAL_INT(0, status0.ac_relay);
    TEST_ASSERT_EQUAL_INT(0, status0.rad_fan);
    TEST_ASSERT_EQUAL_INT(0, status0.fuel_pump);
    TEST_ASSERT_EQUAL_INT(0, status0.purge_valve);
    TEST_ASSERT_EQUAL_INT(0, status0.pinging);
    TEST_ASSERT_EQUAL_INT(0, status0.press_exch);

    mock_data = 255;
    read_io_switches();
    TEST_ASSERT_EQUAL_INT(1, status0.idle_sw);
    TEST_ASSERT_EQUAL_INT(1, status0.ac_sw);
    TEST_ASSERT_EQUAL_INT(1, status0.ac_relay);
    TEST_ASSERT_EQUAL_INT(1, status0.rad_fan);
    TEST_ASSERT_EQUAL_INT(1, status0.fuel_pump);
    TEST_ASSERT_EQUAL_INT(1, status0.purge_valve);
    TEST_ASSERT_EQUAL_INT(1, status0.pinging);
    TEST_ASSERT_EQUAL_INT(1, status0.press_exch);
}

void test_trouble_code_one(void)
{
    mock_data = 0;
    read_trouble_code_one(0);
    TEST_ASSERT_EQUAL_INT(0, status1.crank);
    TEST_ASSERT_EQUAL_INT(0, status1.starter);
    TEST_ASSERT_EQUAL_INT(0, status1.cam);
    TEST_ASSERT_EQUAL_INT(0, status1.inj_1);
    TEST_ASSERT_EQUAL_INT(0, status1.inj_2);
    TEST_ASSERT_EQUAL_INT(0, status1.inj_3);
    TEST_ASSERT_EQUAL_INT(0, status1.inj_4);
    TEST_ASSERT_EQUAL_INT(0, status1.unused_bit_7);

    mock_data = 255;
    read_trouble_code_one(0);
    TEST_ASSERT_EQUAL_INT(1, status1.crank);
    TEST_ASSERT_EQUAL_INT(1, status1.starter);
    TEST_ASSERT_EQUAL_INT(1, status1.cam);
    TEST_ASSERT_EQUAL_INT(1, status1.inj_1);
    TEST_ASSERT_EQUAL_INT(1, status1.inj_2);
    TEST_ASSERT_EQUAL_INT(1, status1.inj_3);
    TEST_ASSERT_EQUAL_INT(1, status1.inj_4);
    TEST_ASSERT_EQUAL_INT(0, status1.unused_bit_7);
}

void test_trouble_code_two(void)
{
    mock_data = 0;
    read_trouble_code_two(0);
    TEST_ASSERT_EQUAL_INT(0, status2.temp);
    TEST_ASSERT_EQUAL_INT(0, status2.knock);
    TEST_ASSERT_EQUAL_INT(0, status2.maf);
    TEST_ASSERT_EQUAL_INT(0, status2.iacv);
    TEST_ASSERT_EQUAL_INT(0, status2.tps);
    TEST_ASSERT_EQUAL_INT(0, status2.oxygen);
    TEST_ASSERT_EQUAL_INT(0, status2.vss);
    TEST_ASSERT_EQUAL_INT(0, status2.purge);

    mock_data = 255;
    read_trouble_code_two(0);
    TEST_ASSERT_EQUAL_INT(1, status2.temp);
    TEST_ASSERT_EQUAL_INT(1, status2.knock);
    TEST_ASSERT_EQUAL_INT(1, status2.maf);
    TEST_ASSERT_EQUAL_INT(1, status2.iacv);
    TEST_ASSERT_EQUAL_INT(1, status2.tps);
    TEST_ASSERT_EQUAL_INT(1, status2.oxygen);
    TEST_ASSERT_EQUAL_INT(1, status2.vss);
    TEST_ASSERT_EQUAL_INT(1, status2.purge);
}

void test_trouble_code_three(void)
{
    mock_data = 0;
    read_trouble_code_three(0);
    TEST_ASSERT_EQUAL_INT(0, status3.fuel_trim);
    TEST_ASSERT_EQUAL_INT(0, status3.idle_sw);
    TEST_ASSERT_EQUAL_INT(0, status3.unused_bit_2);
    TEST_ASSERT_EQUAL_INT(0, status3.wgc);
    TEST_ASSERT_EQUAL_INT(0, status3.baro);
    TEST_ASSERT_EQUAL_INT(0, status3.wrong_maf);
    TEST_ASSERT_EQUAL_INT(0, status3.neutral_sw);
    TEST_ASSERT_EQUAL_INT(0, status3.parking_sw);

    mock_data = 255;
    read_trouble_code_three(0);
    TEST_ASSERT_EQUAL_INT(1, status3.fuel_trim);
    TEST_ASSERT_EQUAL_INT(1, status3.idle_sw);
    TEST_ASSERT_EQUAL_INT(0, status3.unused_bit_2);
    TEST_ASSERT_EQUAL_INT(1, status3.wgc);
    TEST_ASSERT_EQUAL_INT(1, status3.baro);
    TEST_ASSERT_EQUAL_INT(1, status3.wrong_maf);
    TEST_ASSERT_EQUAL_INT(1, status3.neutral_sw);
    TEST_ASSERT_EQUAL_INT(1, status3.parking_sw);
}

int main(int argv, char **argc)
{
    UNITY_BEGIN();

    RUN_TEST(test_read_coolant_temp_index_less_than_14);
    RUN_TEST(test_read_coolant_temp_index_greater_than_256_minus_29);
    RUN_TEST(test_read_coolant_temp_normal_case);
    RUN_TEST(test_battery_voltage);
    RUN_TEST(test_speed);
    RUN_TEST(test_rpm);
    RUN_TEST(test_airflow);
    RUN_TEST(test_throttle_percentage);
    RUN_TEST(test_throttle_signal);
    RUN_TEST(test_mainfold_pressure);
    RUN_TEST(test_boost_control_duty_cycle);
    RUN_TEST(test_ignition_timing);
    RUN_TEST(test_load);
    RUN_TEST(test_injector_pulse_width);
    RUN_TEST(test_iacv_duty_cycle);
    RUN_TEST(test_o2_signal);
    RUN_TEST(test_timing_correction);
    RUN_TEST(test_fuel_trim);
    RUN_TEST(test_atmosphere_pressure);

    RUN_TEST(test_input_switch);
    RUN_TEST(test_in_out_switch);
    RUN_TEST(test_trouble_code_one);
    RUN_TEST(test_trouble_code_two);
    RUN_TEST(test_trouble_code_three);

    UNITY_END();
}