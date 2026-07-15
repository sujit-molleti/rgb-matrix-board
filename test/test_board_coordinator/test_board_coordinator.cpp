#include <Arduino.h>
#include <unity.h>

#include "../../src/layout/services/BoardCoordinator.h"

namespace {
void assertPixelEquals(LEDPixel expected, LEDPixel actual) {
    TEST_ASSERT_EQUAL_UINT8(expected.red, actual.red);
    TEST_ASSERT_EQUAL_UINT8(expected.green, actual.green);
    TEST_ASSERT_EQUAL_UINT8(expected.blue, actual.blue);
}
}

void test_coordinator_begin_starts_on_game_day_layout() {
    BoardCoordinator coordinator(128, 64);

    coordinator.begin(0);

    TEST_ASSERT_EQUAL_STRING("game_day", coordinator.renderer().activeLayoutId());
}

void test_coordinator_tick_before_interval_does_not_switch_layout() {
    BoardCoordinator coordinator(128, 64);

    coordinator.begin(0);

    TEST_ASSERT_FALSE(coordinator.tick(4999));
    TEST_ASSERT_EQUAL_STRING("game_day", coordinator.renderer().activeLayoutId());
}

void test_coordinator_tick_after_interval_switches_to_header_layout() {
    BoardCoordinator coordinator(128, 64);

    coordinator.begin(0);

    TEST_ASSERT_TRUE(coordinator.tick(5000));
    TEST_ASSERT_EQUAL_STRING("game_day_header", coordinator.renderer().activeLayoutId());
}

void test_coordinator_switches_back_to_game_day_layout() {
    BoardCoordinator coordinator(128, 64);

    coordinator.begin(0);
    TEST_ASSERT_TRUE(coordinator.tick(5000));
    TEST_ASSERT_TRUE(coordinator.tick(10000));

    TEST_ASSERT_EQUAL_STRING("game_day", coordinator.renderer().activeLayoutId());
}

void test_coordinator_clears_pixels_between_layout_switches() {
    BoardCoordinator coordinator(128, 64);

    coordinator.begin(0);
    assertPixelEquals({0, 0, 0}, coordinator.renderer().pixelAt(64, 30));

    TEST_ASSERT_TRUE(coordinator.tick(5000));
    assertPixelEquals({255, 255, 255}, coordinator.renderer().pixelAt(64, 30));

    TEST_ASSERT_TRUE(coordinator.tick(10000));
    assertPixelEquals({0, 0, 0}, coordinator.renderer().pixelAt(64, 30));
}

void setup() {
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_coordinator_begin_starts_on_game_day_layout);
    RUN_TEST(test_coordinator_tick_before_interval_does_not_switch_layout);
    RUN_TEST(test_coordinator_tick_after_interval_switches_to_header_layout);
    RUN_TEST(test_coordinator_switches_back_to_game_day_layout);
    RUN_TEST(test_coordinator_clears_pixels_between_layout_switches);
    UNITY_END();
}

void loop() {
}
