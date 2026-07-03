#pragma once

#include "../models/DisplayLEDPixelBuffer.h"
#include "../models/GlobalLEDPixelBuffer.h"
#include "../models/LEDPixel.h"
#include "validation/ValidationResult.h"

class LayoutValidator;

class LayoutRenderer {
public:
  LayoutRenderer(int boardWidth, int boardHeight);

  void clear(LEDPixel pixel = {0, 0, 0});
  void setPixel(int x, int y, LEDPixel pixel);
  void renderDisplay(const char* displayId, const DisplayLEDPixelBuffer& displayBuffer);
  LEDPixel pixelAt(int x, int y) const;
  ValidationResult validate(const LayoutValidator& validator) const;

private:
  int boardWidth;
  int boardHeight;
  GlobalLEDPixelBuffer pixelBuffer;
};
