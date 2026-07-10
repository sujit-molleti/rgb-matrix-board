#pragma once

#include "../models/DisplayLEDPixelBuffer.h"
#include "../models/GlobalLEDPixelBuffer.h"
#include "../models/LEDPixel.h"
#include "../models/VirtualRgbBoard.h"
#include "validation/ValidationResult.h"

class LayoutValidator;
struct Display;

class LayoutRenderer {
public:
  LayoutRenderer(int boardWidth, int boardHeight);

  void clear(LEDPixel pixel = {0, 0, 0});
  void renderDisplayBorders();
  void setPixel(int x, int y, LEDPixel pixel);
  void renderDisplay(const char* displayId, const DisplayLEDPixelBuffer& displayBuffer);
  bool setActiveLayout(const char* layoutId);
  const char* activeLayoutId() const;
  LEDPixel pixelAt(int x, int y) const;
  ValidationResult validate(const LayoutValidator& validator) const;

private:
  VirtualRgbBoard board;
  std::string currentLayoutId;
  GlobalLEDPixelBuffer pixelBuffer;

  const Layout* activeLayout() const;
  DisplayLEDPixelBuffer displayLEDPixelBufferFor(const Display& display) const;
};
