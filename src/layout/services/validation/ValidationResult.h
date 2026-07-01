#pragma once

#include <vector>

#include "ValidationError.h"

struct ValidationResult {
  bool isValid = true;
  std::vector<ValidationError> errors;

  void addError(const ValidationError& error);
};
