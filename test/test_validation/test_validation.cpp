#include <Arduino.h>
#include <unity.h>

#include "../../src/layout/models/VirtualRgbBoard.h"
#include "../../src/layout/services/LayoutValidator.h"

namespace {
Display makeDisplay(const char* id, int x, int y, int width, int height) {
    return {id, DisplayRole::Primary, {x, y, width, height}};
}

Layout makeFullLayout(const char* id, int width, int height) {
    Layout layout;
    layout.id = id;
    layout.name = id;
    layout.displays.push_back(makeDisplay("full", 0, 0, width, height));
    return layout;
}

VirtualRgbBoard makeValidBoard() {
    VirtualRgbBoard board;
    board.width = 2;
    board.height = 2;
    board.defaultLayoutId = "main";
    board.layouts.push_back(makeFullLayout("main", 2, 2));
    return board;
}

bool hasErrorType(const ValidationResult& result, ValidationErrorType type) {
    for (const ValidationError& error : result.errors) {
        if (error.type == type) {
            return true;
        }
    }

    return false;
}
}

void test_valid_layout_has_no_errors() {
    VirtualRgbBoard board = makeValidBoard();

    ValidationResult result = LayoutValidator(2, 2).validate(board);

    TEST_ASSERT_TRUE(result.isValid);
    TEST_ASSERT_EQUAL_UINT(0, result.errors.size());
}

void test_invalid_board_size_is_reported() {
    VirtualRgbBoard board = makeValidBoard();

    ValidationResult result = LayoutValidator(3, 2).validate(board);

    TEST_ASSERT_FALSE(result.isValid);
    TEST_ASSERT_TRUE(hasErrorType(result, ValidationErrorType::InvalidBoardSize));
}

void test_missing_default_layout_is_reported() {
    VirtualRgbBoard board = makeValidBoard();
    board.defaultLayoutId = "missing";

    ValidationResult result = LayoutValidator(2, 2).validate(board);

    TEST_ASSERT_FALSE(result.isValid);
    TEST_ASSERT_TRUE(hasErrorType(result, ValidationErrorType::MissingDefaultLayout));
}

void test_duplicate_layout_id_is_reported() {
    VirtualRgbBoard board = makeValidBoard();
    board.layouts.push_back(makeFullLayout("main", 2, 2));

    ValidationResult result = LayoutValidator(2, 2).validate(board);

    TEST_ASSERT_FALSE(result.isValid);
    TEST_ASSERT_TRUE(hasErrorType(result, ValidationErrorType::DuplicateLayoutId));
}

void test_duplicate_display_id_is_reported() {
    VirtualRgbBoard board;
    board.width = 2;
    board.height = 2;
    board.defaultLayoutId = "main";

    Layout layout;
    layout.id = "main";
    layout.displays.push_back(makeDisplay("dup", 0, 0, 2, 1));
    layout.displays.push_back(makeDisplay("dup", 0, 1, 2, 1));
    board.layouts.push_back(layout);

    ValidationResult result = LayoutValidator(2, 2).validate(board);

    TEST_ASSERT_FALSE(result.isValid);
    TEST_ASSERT_TRUE(hasErrorType(result, ValidationErrorType::DuplicateDisplayId));
}

void test_invalid_display_size_is_reported() {
    VirtualRgbBoard board = makeValidBoard();
    board.layouts[0].displays[0].bounds.width = 0;

    ValidationResult result = LayoutValidator(2, 2).validate(board);

    TEST_ASSERT_FALSE(result.isValid);
    TEST_ASSERT_TRUE(hasErrorType(result, ValidationErrorType::InvalidDisplaySize));
}

void test_out_of_bounds_display_is_reported() {
    VirtualRgbBoard board = makeValidBoard();
    board.layouts[0].displays[0].bounds.x = 1;

    ValidationResult result = LayoutValidator(2, 2).validate(board);

    TEST_ASSERT_FALSE(result.isValid);
    TEST_ASSERT_TRUE(hasErrorType(result, ValidationErrorType::DisplayOutOfBounds));
}

void test_unmapped_virtual_pixel_is_reported() {
    VirtualRgbBoard board = makeValidBoard();
    board.layouts[0].displays[0].bounds.width = 1;

    ValidationResult result = LayoutValidator(2, 2).validate(board);

    TEST_ASSERT_FALSE(result.isValid);
    TEST_ASSERT_TRUE(hasErrorType(result, ValidationErrorType::UnmappedVirtualPixel));
}

void test_overlapping_display_pixel_is_reported() {
    VirtualRgbBoard board = makeValidBoard();
    board.layouts[0].displays.push_back(makeDisplay("overlap", 1, 1, 1, 1));

    ValidationResult result = LayoutValidator(2, 2).validate(board);

    TEST_ASSERT_FALSE(result.isValid);
    TEST_ASSERT_TRUE(hasErrorType(result, ValidationErrorType::OverlappingDisplayPixel));
}

void setup() {
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_valid_layout_has_no_errors);
    RUN_TEST(test_invalid_board_size_is_reported);
    RUN_TEST(test_missing_default_layout_is_reported);
    RUN_TEST(test_duplicate_layout_id_is_reported);
    RUN_TEST(test_duplicate_display_id_is_reported);
    RUN_TEST(test_invalid_display_size_is_reported);
    RUN_TEST(test_out_of_bounds_display_is_reported);
    RUN_TEST(test_unmapped_virtual_pixel_is_reported);
    RUN_TEST(test_overlapping_display_pixel_is_reported);
    UNITY_END();
}

void loop() {
}
