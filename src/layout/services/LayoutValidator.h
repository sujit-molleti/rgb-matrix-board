#pragma once

#include "../models/ValidationResult.h"

struct VirtualRgbBoard;

class LayoutValidator {
public:
  LayoutValidator(int expectedWidth, int expectedHeight);

  ValidationResult validate(const VirtualRgbBoard& board) const;

private:
  int expectedWidth;
  int expectedHeight;
};
