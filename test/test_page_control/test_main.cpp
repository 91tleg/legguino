#include <unity.h>
#include "state_control.h"
#include "state.h"

void setUp(void) 
{
    // Reset before each test
    scan_curr_page = SCAN::SENSOR_ONE;
    diag_curr_page = DIAG::IN;
    params_curr_page = PARAMS::PARAMS_ONE;
    race_curr_page = RACE::TW;
}

void tearDown(void) {}

bool btn1_pressed_called = true;  // Mocking button press
bool btn1_pressed(void) 
{
    return btn1_pressed_called;
}

void test_scan_page_control(void)  
{
    scan_page_control();
    TEST_ASSERT_EQUAL(SCAN::SENSOR_TWO, scan_curr_page);

    scan_curr_page = SCAN::CLEAR_TC;
    scan_page_control();
    TEST_ASSERT_EQUAL(SCAN::SENSOR_ONE, scan_curr_page);
}

void test_params_page_control(void)
{
    params_page_control();
    TEST_ASSERT_EQUAL(PARAMS::PARAMS_TWO, params_curr_page);

    params_curr_page = PARAMS::PARAMS_EIGHT;
    params_page_control();
    TEST_ASSERT_EQUAL(PARAMS::PARAMS_ONE, params_curr_page);
}

void test_diag_page_control(void) 
{
    diag_page_control();
    TEST_ASSERT_EQUAL(DIAG::IO, diag_curr_page);

    diag_curr_page = DIAG::CLEAR;
    diag_page_control();
    TEST_ASSERT_EQUAL(DIAG::IN, diag_curr_page);
}

void test_race_page_control(void)
{
    race_page_control();
    TEST_ASSERT_EQUAL(RACE::TPS, race_curr_page);

    race_curr_page = RACE::MP;
    race_page_control();
    TEST_ASSERT_EQUAL(RACE::TW, race_curr_page);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_scan_page_control);
    RUN_TEST(test_diag_page_control);
    RUN_TEST(test_params_page_control);
    RUN_TEST(test_race_page_control);

    UNITY_END();
}