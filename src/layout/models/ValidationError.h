#pragma once

#include <string>

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

const char* validationErrorTypeName(ValidationErrorType type);
