#pragma once

#include <Adafruit_GFX.h>

#include "../models/DisplayLEDPixelBuffer.h"
#include "../models/LEDPixel.h"

class DisplayContentDrawer : public Adafruit_GFX {
public:
  explicit DisplayContentDrawer(DisplayLEDPixelBuffer& displayBuffer);

  void drawPixel(int16_t x, int16_t y, uint16_t color) override;
  void drawText(int16_t x, int16_t y, const char* text, LEDPixel pixel);
  void drawTextFitted(const char* text, LEDPixel pixel);

private:
  DisplayLEDPixelBuffer& displayBuffer;

  uint16_t rgb565(LEDPixel pixel) const;
  LEDPixel ledPixelFromRgb565(uint16_t color) const;
};
