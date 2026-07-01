#include "layout.h"

#include <stdint.h>

namespace {
const size_t MaxValidationErrors = 32;

bool sameId(const std::string& left, const std::string& right) {
  return left == right;
}

bool hasErrorCapacity(const ValidationResult& result) {
  return result.errors.size() < MaxValidationErrors;
}
}

void ValidationResult::addError(const ValidationError& error) {
  isValid = false;

  if (hasErrorCapacity(*this)) {
    errors.push_back(error);
  }
}

ValidationError::ValidationError(
  ValidationErrorType type,
  const std::string& message,
  const std::string& layoutId,
  const std::string& displayId,
  int x,
  int y
) : type(type),
    message(message),
    layoutId(layoutId),
    displayId(displayId),
    x(x),
    y(y) {
}

LayoutValidator::LayoutValidator(int expectedWidth, int expectedHeight)
  : expectedWidth(expectedWidth),
    expectedHeight(expectedHeight) {
}

ValidationResult LayoutValidator::validate(const VirtualRgbBoard& board) const {
  ValidationResult result;

  if (board.width != expectedWidth || board.height != expectedHeight) {
    result.addError(ValidationError(
      ValidationErrorType::InvalidBoardSize,
      "Board dimensions do not match firmware dimensions",
      "",
      "",
      -1,
      -1
    ));
  }

  validateLayoutIds(board, result);
  validateDefaultLayout(board, result);

  for (const Layout& layout : board.layouts) {
    validateLayoutCoverage(board, layout, result);
  }

  return result;
}

void LayoutValidator::validateLayoutIds(
  const VirtualRgbBoard& board,
  ValidationResult& result
) const {
  for (size_t i = 0; i < board.layouts.size(); i++) {
    for (size_t j = i + 1; j < board.layouts.size(); j++) {
      if (sameId(board.layouts[i].id, board.layouts[j].id)) {
        result.addError(ValidationError(
          ValidationErrorType::DuplicateLayoutId,
          "Layout id is duplicated",
          board.layouts[i].id,
          "",
          -1,
          -1
        ));
      }
    }
  }
}

void LayoutValidator::validateDefaultLayout(
  const VirtualRgbBoard& board,
  ValidationResult& result
) const {
  for (const Layout& layout : board.layouts) {
    if (sameId(layout.id, board.defaultLayoutId)) {
      return;
    }
  }

  result.addError(ValidationError(
    ValidationErrorType::MissingDefaultLayout,
    "Default layout id does not match any layout",
    board.defaultLayoutId,
    "",
    -1,
    -1
  ));
}

void LayoutValidator::validateLayoutCoverage(
  const VirtualRgbBoard& board,
  const Layout& layout,
  ValidationResult& result
) const {
  if (board.width <= 0 || board.height <= 0) {
    return;
  }

  std::vector<uint8_t> coverage(board.width * board.height, 0);

  for (size_t i = 0; i < layout.displays.size(); i++) {
    const Display& display = layout.displays[i];

    for (size_t j = i + 1; j < layout.displays.size(); j++) {
      if (sameId(display.id, layout.displays[j].id)) {
        result.addError(ValidationError(
          ValidationErrorType::DuplicateDisplayId,
          "Display id is duplicated within layout",
          layout.id,
          display.id,
          -1,
          -1
        ));
      }
    }

    if (display.bounds.width <= 0 || display.bounds.height <= 0) {
      result.addError(ValidationError(
        ValidationErrorType::InvalidDisplaySize,
        "Display width and height must be positive",
        layout.id,
        display.id,
        display.bounds.x,
        display.bounds.y
      ));
      continue;
    }

    const int right = display.bounds.x + display.bounds.width;
    const int bottom = display.bounds.y + display.bounds.height;
    const bool outOfBounds =
      display.bounds.x < 0 ||
      display.bounds.y < 0 ||
      right > board.width ||
      bottom > board.height;

    if (outOfBounds) {
      result.addError(ValidationError(
        ValidationErrorType::DisplayOutOfBounds,
        "Display bounds must stay inside the board",
        layout.id,
        display.id,
        display.bounds.x,
        display.bounds.y
      ));
      continue;
    }

    for (int y = display.bounds.y; y < bottom; y++) {
      for (int x = display.bounds.x; x < right; x++) {
        const int index = y * board.width + x;
        coverage[index]++;
      }
    }
  }

  for (int y = 0; y < board.height; y++) {
    for (int x = 0; x < board.width; x++) {
      const int index = y * board.width + x;

      if (coverage[index] == 0) {
        result.addError(ValidationError(
          ValidationErrorType::UnmappedVirtualPixel,
          "Virtual pixel is not covered by any display",
          layout.id,
          "",
          x,
          y
        ));
      } else if (coverage[index] > 1) {
        result.addError(ValidationError(
          ValidationErrorType::OverlappingDisplayPixel,
          "Virtual pixel is covered by multiple displays",
          layout.id,
          "",
          x,
          y
        ));
      }
    }
  }
}

ValidationResult VirtualRgbBoard::validate(const LayoutValidator& validator) const {
  return validator.validate(*this);
}

VirtualRgbBoard createDefaultVirtualRgbBoard() {
  return {
    128,
    64,
    "game_day",
    {
      {
        "game_day",
        "Game Day",
        {
          {"primary", DisplayRole::Primary, {0, 0, 128, 40}},
          {"secondary_1", DisplayRole::Secondary, {0, 40, 32, 16}},
          {"secondary_2", DisplayRole::Secondary, {32, 40, 32, 16}},
          {"secondary_3", DisplayRole::Secondary, {64, 40, 32, 16}},
          {"secondary_4", DisplayRole::Secondary, {96, 40, 32, 16}},
          {"ticker", DisplayRole::Ticker, {0, 56, 128, 8}}
        }
      }
    }
  };
}

const char* validationErrorTypeName(ValidationErrorType type) {
  switch (type) {
    case ValidationErrorType::InvalidBoardSize:
      return "InvalidBoardSize";
    case ValidationErrorType::MissingDefaultLayout:
      return "MissingDefaultLayout";
    case ValidationErrorType::DuplicateLayoutId:
      return "DuplicateLayoutId";
    case ValidationErrorType::DuplicateDisplayId:
      return "DuplicateDisplayId";
    case ValidationErrorType::InvalidDisplaySize:
      return "InvalidDisplaySize";
    case ValidationErrorType::DisplayOutOfBounds:
      return "DisplayOutOfBounds";
    case ValidationErrorType::UnmappedVirtualPixel:
      return "UnmappedVirtualPixel";
    case ValidationErrorType::OverlappingDisplayPixel:
      return "OverlappingDisplayPixel";
    default:
      return "Unknown";
  }
}
