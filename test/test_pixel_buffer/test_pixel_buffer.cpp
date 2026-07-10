#include <Arduino.h>
#include <unity.h>

#include "../../src/layout/models/DisplayLEDPixelBuffer.h"
#include "../../src/layout/models/GlobalLEDPixelBuffer.h"

namespace {
void assertPixelEquals(LEDPixel expected, LEDPixel actual) {
    TEST_ASSERT_EQUAL_UINT8(expected.red, actual.red);
    TEST_ASSERT_EQUAL_UINT8(expected.green, actual.green);
    TEST_ASSERT_EQUAL_UINT8(expected.blue, actual.blue);
}
}

void test_display_buffer_reports_dimensions() {
    DisplayLEDPixelBuffer buffer(4, 3);

    TEST_ASSERT_EQUAL_INT(4, buffer.getWidth());
    TEST_ASSERT_EQUAL_INT(3, buffer.getHeight());
}

void test_display_buffer_defaults_to_black() {
    DisplayLEDPixelBuffer buffer(2, 2);

    assertPixelEquals({0, 0, 0}, buffer.getPixel(1, 1));
}

void test_display_buffer_sets_and_gets_pixel() {
    DisplayLEDPixelBuffer buffer(4, 3);

    buffer.setPixel(2, 1, {10, 20, 30});

    assertPixelEquals({10, 20, 30}, buffer.getPixel(2, 1));
}

void test_display_buffer_clear_sets_every_pixel() {
    DisplayLEDPixelBuffer buffer(2, 2);

    buffer.setPixel(0, 0, {1, 2, 3});
    buffer.clear({9, 8, 7});

    assertPixelEquals({9, 8, 7}, buffer.getPixel(0, 0));
    assertPixelEquals({9, 8, 7}, buffer.getPixel(1, 1));
}

void test_display_buffer_ignores_out_of_bounds_writes() {
    DisplayLEDPixelBuffer buffer(2, 2);

    buffer.setPixel(-1, 0, {255, 0, 0});
    buffer.setPixel(2, 1, {0, 255, 0});
    buffer.setPixel(1, 2, {0, 0, 255});

    assertPixelEquals({0, 0, 0}, buffer.getPixel(0, 0));
    assertPixelEquals({0, 0, 0}, buffer.getPixel(1, 1));
}

void test_display_buffer_out_of_bounds_reads_return_black() {
    DisplayLEDPixelBuffer buffer(2, 2);

    assertPixelEquals({0, 0, 0}, buffer.getPixel(-1, 0));
    assertPixelEquals({0, 0, 0}, buffer.getPixel(2, 0));
    assertPixelEquals({0, 0, 0}, buffer.getPixel(0, 2));
}

void test_global_buffer_sets_clears_and_bounds_checks_pixels() {
    GlobalLEDPixelBuffer buffer(3, 2);

    buffer.setPixel(2, 1, {4, 5, 6});
    buffer.setPixel(3, 1, {255, 255, 255});

    assertPixelEquals({4, 5, 6}, buffer.getPixel(2, 1));
    assertPixelEquals({0, 0, 0}, buffer.getPixel(3, 1));

    buffer.clear({7, 8, 9});

    assertPixelEquals({7, 8, 9}, buffer.getPixel(0, 0));
    assertPixelEquals({7, 8, 9}, buffer.getPixel(2, 1));
}

void setup() {
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_display_buffer_reports_dimensions);
    RUN_TEST(test_display_buffer_defaults_to_black);
    RUN_TEST(test_display_buffer_sets_and_gets_pixel);
    RUN_TEST(test_display_buffer_clear_sets_every_pixel);
    RUN_TEST(test_display_buffer_ignores_out_of_bounds_writes);
    RUN_TEST(test_display_buffer_out_of_bounds_reads_return_black);
    RUN_TEST(test_global_buffer_sets_clears_and_bounds_checks_pixels);
    UNITY_END();
}

void loop() {
}
