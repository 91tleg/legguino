#include <unity.h>
#include "btn.h"

void setUp(void) {}
void tearDown(void) {}

void btn1_read(void) {}
void btn2_read(void) {}

void test_button_one_is_pressed(void)
{
    btn1 = HIGH;
    btn1_prev = HIGH;
    TEST_ASSERT_FALSE(btn1_pressed()); // Not pressed

    btn1 = LOW;
    TEST_ASSERT_TRUE(btn1_pressed()); // Should detect press

    TEST_ASSERT_FALSE(btn1_pressed()); // Shouldn't trigger again

    btn1 = HIGH;
    TEST_ASSERT_FALSE(btn1_pressed()); // No press

    btn1 = LOW;
    TEST_ASSERT_TRUE(btn1_pressed()); // Should detect press again
}

void test_button_two_is_pressed(void)
{
    btn2 = HIGH;
    btn2_prev = HIGH;
    TEST_ASSERT_FALSE(btn2_pressed()); // Not pressed

    btn2 = LOW;
    TEST_ASSERT_TRUE(btn2_pressed()); // Should detect press

    TEST_ASSERT_FALSE(btn2_pressed()); // Shouldn't trigger again

    btn2 = HIGH;
    TEST_ASSERT_FALSE(btn2_pressed()); // No press

    btn2 = LOW;
    TEST_ASSERT_TRUE(btn2_pressed()); // Should detect press again
}

int main(int argv, char **argc)
{
    UNITY_BEGIN();
    RUN_TEST(test_button_one_is_pressed);
    RUN_TEST(test_button_two_is_pressed);
    UNITY_END();
}