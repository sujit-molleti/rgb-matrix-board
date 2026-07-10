#include <Arduino.h>
#include <unity.h>

#include "../../src/layout/services/LayoutConfigLoader.h"

void test_loads_board_and_displays_from_json() {
    const char* json = R"JSON(
        {
          "width": 8,
          "height": 4,
          "defaultLayoutId": "main",
          "layouts": [
            {
              "id": "main",
              "name": "Main",
              "displays": [
                {
                  "id": "primary",
                  "role": "primary",
                  "bounds": { "x": 0, "y": 0, "width": 8, "height": 2 }
                },
                {
                  "id": "ticker",
                  "role": "ticker",
                  "bounds": { "x": 0, "y": 2, "width": 8, "height": 2 }
                }
              ]
            }
          ]
        }
    )JSON";

    VirtualRgbBoard board = LayoutConfigLoader().load(json);

    TEST_ASSERT_EQUAL_INT(8, board.width);
    TEST_ASSERT_EQUAL_INT(4, board.height);
    TEST_ASSERT_EQUAL_STRING("main", board.defaultLayoutId.c_str());
    TEST_ASSERT_EQUAL_UINT(1, board.layouts.size());
    TEST_ASSERT_EQUAL_STRING("main", board.layouts[0].id.c_str());
    TEST_ASSERT_EQUAL_STRING("Main", board.layouts[0].name.c_str());
    TEST_ASSERT_EQUAL_UINT(2, board.layouts[0].displays.size());

    const Display& primary = board.layouts[0].displays[0];
    TEST_ASSERT_EQUAL_STRING("primary", primary.id.c_str());
    TEST_ASSERT_EQUAL_INT(static_cast<int>(DisplayRole::Primary), static_cast<int>(primary.role));
    TEST_ASSERT_EQUAL_INT(0, primary.bounds.x);
    TEST_ASSERT_EQUAL_INT(0, primary.bounds.y);
    TEST_ASSERT_EQUAL_INT(8, primary.bounds.width);
    TEST_ASSERT_EQUAL_INT(2, primary.bounds.height);

    const Display& ticker = board.layouts[0].displays[1];
    TEST_ASSERT_EQUAL_STRING("ticker", ticker.id.c_str());
    TEST_ASSERT_EQUAL_INT(static_cast<int>(DisplayRole::Ticker), static_cast<int>(ticker.role));
}

void test_loads_secondary_header_and_unknown_roles() {
    const char* json = R"JSON(
        {
          "width": 3,
          "height": 3,
          "defaultLayoutId": "roles",
          "layouts": [
            {
              "id": "roles",
              "name": "Roles",
              "displays": [
                { "id": "secondary", "role": "secondary", "bounds": { "x": 0, "y": 0, "width": 1, "height": 1 } },
                { "id": "header", "role": "header", "bounds": { "x": 1, "y": 0, "width": 1, "height": 1 } },
                { "id": "unknown", "role": "clock", "bounds": { "x": 2, "y": 0, "width": 1, "height": 1 } }
              ]
            }
          ]
        }
    )JSON";

    VirtualRgbBoard board = LayoutConfigLoader().load(json);

    TEST_ASSERT_EQUAL_INT(static_cast<int>(DisplayRole::Secondary), static_cast<int>(board.layouts[0].displays[0].role));
    TEST_ASSERT_EQUAL_INT(static_cast<int>(DisplayRole::Header), static_cast<int>(board.layouts[0].displays[1].role));
    TEST_ASSERT_EQUAL_INT(static_cast<int>(DisplayRole::Unknown), static_cast<int>(board.layouts[0].displays[2].role));
}

void test_missing_fields_use_defaults() {
    const char* json = R"JSON(
        {
          "layouts": [
            {
              "displays": [
                { "bounds": {} },
                { "id": "missing_role" }
              ]
            }
          ]
        }
    )JSON";

    VirtualRgbBoard board = LayoutConfigLoader().load(json);

    TEST_ASSERT_EQUAL_INT(0, board.width);
    TEST_ASSERT_EQUAL_INT(0, board.height);
    TEST_ASSERT_EQUAL_STRING("", board.defaultLayoutId.c_str());
    TEST_ASSERT_EQUAL_UINT(1, board.layouts.size());
    TEST_ASSERT_EQUAL_STRING("", board.layouts[0].id.c_str());
    TEST_ASSERT_EQUAL_STRING("", board.layouts[0].name.c_str());
    TEST_ASSERT_EQUAL_UINT(2, board.layouts[0].displays.size());
    TEST_ASSERT_EQUAL_STRING("", board.layouts[0].displays[0].id.c_str());
    TEST_ASSERT_EQUAL_INT(static_cast<int>(DisplayRole::Unknown), static_cast<int>(board.layouts[0].displays[0].role));
    TEST_ASSERT_EQUAL_INT(0, board.layouts[0].displays[0].bounds.width);
    TEST_ASSERT_EQUAL_STRING("missing_role", board.layouts[0].displays[1].id.c_str());
    TEST_ASSERT_EQUAL_INT(static_cast<int>(DisplayRole::Unknown), static_cast<int>(board.layouts[0].displays[1].role));
}

void test_invalid_json_returns_empty_board() {
    VirtualRgbBoard board = LayoutConfigLoader().load("{ invalid json");

    TEST_ASSERT_EQUAL_INT(0, board.width);
    TEST_ASSERT_EQUAL_INT(0, board.height);
    TEST_ASSERT_EQUAL_STRING("", board.defaultLayoutId.c_str());
    TEST_ASSERT_EQUAL_UINT(0, board.layouts.size());
}

void setup() {
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_loads_board_and_displays_from_json);
    RUN_TEST(test_loads_secondary_header_and_unknown_roles);
    RUN_TEST(test_missing_fields_use_defaults);
    RUN_TEST(test_invalid_json_returns_empty_board);
    UNITY_END();
}

void loop() {
}
