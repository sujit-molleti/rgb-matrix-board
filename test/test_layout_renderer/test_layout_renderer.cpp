#include <Arduino.h>
#include <unity.h>

#include "../../src/layout/services/LayoutRenderer.h"
#include "../../src/layout/services/LayoutValidator.h"

namespace {
void assertPixelEquals(LEDPixel expected, LEDPixel actual) {
    TEST_ASSERT_EQUAL_UINT8(expected.red, actual.red);
    TEST_ASSERT_EQUAL_UINT8(expected.green, actual.green);
    TEST_ASSERT_EQUAL_UINT8(expected.blue, actual.blue);
}
}

void test_renderer_uses_default_layout_from_config() {
    LayoutRenderer renderer(128, 64);

    TEST_ASSERT_EQUAL_STRING("game_day", renderer.activeLayoutId());
}

void test_renderer_rejects_missing_or_null_layouts() {
    LayoutRenderer renderer(128, 64);

    TEST_ASSERT_FALSE(renderer.setActiveLayout(nullptr));
    TEST_ASSERT_FALSE(renderer.setActiveLayout("missing"));
    TEST_ASSERT_EQUAL_STRING("game_day", renderer.activeLayoutId());
}

void test_renderer_can_switch_to_existing_layout() {
    LayoutRenderer renderer(128, 64);

    TEST_ASSERT_TRUE(renderer.setActiveLayout("game_day_header"));
    TEST_ASSERT_EQUAL_STRING("game_day_header", renderer.activeLayoutId());
}

void test_renderer_set_pixel_and_clear_update_global_buffer() {
    LayoutRenderer renderer(128, 64);

    renderer.setPixel(4, 5, {11, 22, 33});
    assertPixelEquals({11, 22, 33}, renderer.pixelAt(4, 5));

    renderer.clear({1, 2, 3});
    assertPixelEquals({1, 2, 3}, renderer.pixelAt(4, 5));
    assertPixelEquals({1, 2, 3}, renderer.pixelAt(0, 0));
}

void test_renderer_renders_display_buffer_into_display_bounds() {
    LayoutRenderer renderer(128, 64);
    DisplayLEDPixelBuffer displayBuffer(32, 16);

    displayBuffer.setPixel(2, 3, {100, 110, 120});

    renderer.renderDisplay("secondary_2", displayBuffer);

    assertPixelEquals({100, 110, 120}, renderer.pixelAt(34, 43));
    assertPixelEquals({0, 0, 0}, renderer.pixelAt(2, 3));
}

void test_renderer_ignores_missing_display_id() {
    LayoutRenderer renderer(128, 64);
    DisplayLEDPixelBuffer displayBuffer(32, 16);

    displayBuffer.setPixel(0, 0, {200, 0, 0});
    renderer.renderDisplay("missing", displayBuffer);

    assertPixelEquals({0, 0, 0}, renderer.pixelAt(0, 40));
}

void test_renderer_ignores_display_buffer_with_wrong_size() {
    LayoutRenderer renderer(128, 64);
    DisplayLEDPixelBuffer displayBuffer(31, 16);

    displayBuffer.setPixel(0, 0, {0, 200, 0});
    renderer.renderDisplay("secondary_2", displayBuffer);

    assertPixelEquals({0, 0, 0}, renderer.pixelAt(32, 40));
}

void test_renderer_validates_loaded_layout_config() {
    LayoutRenderer renderer(128, 64);

    ValidationResult result = renderer.validate(LayoutValidator(128, 64));

    TEST_ASSERT_TRUE(result.isValid);
    TEST_ASSERT_EQUAL_UINT(0, result.errors.size());
}

void setup() {
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_renderer_uses_default_layout_from_config);
    RUN_TEST(test_renderer_rejects_missing_or_null_layouts);
    RUN_TEST(test_renderer_can_switch_to_existing_layout);
    RUN_TEST(test_renderer_set_pixel_and_clear_update_global_buffer);
    RUN_TEST(test_renderer_renders_display_buffer_into_display_bounds);
    RUN_TEST(test_renderer_ignores_missing_display_id);
    RUN_TEST(test_renderer_ignores_display_buffer_with_wrong_size);
    RUN_TEST(test_renderer_validates_loaded_layout_config);
    UNITY_END();
}

void loop() {
}
