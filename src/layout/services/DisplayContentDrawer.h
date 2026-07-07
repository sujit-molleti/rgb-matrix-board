#pragma once

#include <Adafruit_GFX.h>

#include "../models/DisplayLEDPixelBuffer.h"
#include "../models/LEDPixel.h"

class DisplayContentDrawer {
public:
  explicit DisplayContentDrawer(DisplayLEDPixelBuffer& displayBuffer);

  void drawChar(int x, int y, char character, LEDPixel pixel);
  void drawText(int x, int y, const char* text, LEDPixel pixel);

private:
  DisplayLEDPixelBuffer& displayBuffer;

  unsigned char glyphRow(char character, int row) const;
};
