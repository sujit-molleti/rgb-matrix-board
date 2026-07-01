#pragma once

#include "../models/LEDPixel.h"
#include "../models/LEDPixelBuffer.h"
#include "../models/ValidationResult.h"

class LayoutValidator;

class LayoutRenderer {
public:
  LayoutRenderer(int boardWidth, int boardHeight);

  void clear(LEDPixel pixel = {0, 0, 0});
  void setPixel(int x, int y, LEDPixel pixel);
  LEDPixel pixelAt(int x, int y) const;
  ValidationResult validate(const LayoutValidator& validator) const;

private:
  int boardWidth;
  int boardHeight;
  LEDPixelBuffer pixelBuffer;
};
