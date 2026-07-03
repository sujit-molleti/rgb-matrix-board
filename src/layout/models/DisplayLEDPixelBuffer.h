#pragma once

#include <vector>

#include "LEDPixel.h"

class DisplayLEDPixelBuffer {
public:
  DisplayLEDPixelBuffer(int width, int height);

  int getWidth() const;
  int getHeight() const;
  void clear(LEDPixel pixel = {0, 0, 0});
  void setPixel(int x, int y, LEDPixel pixel);
  LEDPixel getPixel(int x, int y) const;

private:
  int width;
  int height;
  std::vector<LEDPixel> pixels;

  bool contains(int x, int y) const;
  int pixelIndex(int x, int y) const;
};
