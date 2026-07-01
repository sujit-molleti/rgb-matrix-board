#include "ValidationError.h"

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
