#pragma once

#include <string>
#include <vector>

struct Rect {
  int x;
  int y;
  int width;
  int height;
};

enum class DisplayRole {
  Primary,
  Secondary,
  Ticker,
  Unknown
};

struct Display {
  std::string id;
  DisplayRole role;
  Rect bounds;
};

struct Layout {
  std::string id;
  std::string name;
  std::vector<Display> displays;
};

enum class ValidationErrorType {
  InvalidBoardSize,
  MissingDefaultLayout,
  DuplicateLayoutId,
  DuplicateDisplayId,
  InvalidDisplaySize,
  DisplayOutOfBounds,
  UnmappedVirtualPixel,
  OverlappingDisplayPixel
};

struct ValidationError {
  ValidationError(
    ValidationErrorType type,
    const std::string& message,
    const std::string& layoutId = "",
    const std::string& displayId = "",
    int x = -1,
    int y = -1
  );

  ValidationErrorType type;
  std::string message;
  std::string layoutId;
  std::string displayId;
  int x;
  int y;
};

struct ValidationResult {
  bool isValid = true;
  std::vector<ValidationError> errors;

  void addError(const ValidationError& error);
};

class LayoutValidator;

struct VirtualRgbBoard {
  int width;
  int height;
  std::string defaultLayoutId;
  std::vector<Layout> layouts;

  ValidationResult validate(const LayoutValidator& validator) const;
};

class LayoutValidator {
public:
  LayoutValidator(int expectedWidth, int expectedHeight);

  ValidationResult validate(const VirtualRgbBoard& board) const;

private:
  int expectedWidth;
  int expectedHeight;

  void validateLayoutIds(
    const VirtualRgbBoard& board,
    ValidationResult& result
  ) const;

  void validateDefaultLayout(
    const VirtualRgbBoard& board,
    ValidationResult& result
  ) const;

  void validateLayoutCoverage(
    const VirtualRgbBoard& board,
    const Layout& layout,
    ValidationResult& result
  ) const;
};

VirtualRgbBoard createDefaultVirtualRgbBoard();
const char* validationErrorTypeName(ValidationErrorType type);
