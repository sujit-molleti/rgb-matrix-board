#include "ValidationResult.h"

namespace {
const size_t MaxValidationErrors = 32;
}

void ValidationResult::addError(const ValidationError& error) {
  isValid = false;

  if (errors.size() < MaxValidationErrors) {
    errors.push_back(error);
  }
}
