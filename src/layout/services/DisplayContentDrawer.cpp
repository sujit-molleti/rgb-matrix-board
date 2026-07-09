#include "DisplayContentDrawer.h"

DisplayContentDrawer::DisplayContentDrawer(DisplayLEDPixelBuffer& displayBuffer)
  : Adafruit_GFX(displayBuffer.getWidth(), displayBuffer.getHeight()),
    displayBuffer(displayBuffer) {
}

void DisplayContentDrawer::drawPixel(
  int16_t x,
  int16_t y,
  uint16_t color
) {
  displayBuffer.setPixel(x, y, ledPixelFromRgb565(color));
}

void DisplayContentDrawer::drawText(
  int16_t x,
  int16_t y,
  const char* text,
  LEDPixel pixel
) {
  setCursor(x, y);
  setTextColor(rgb565(pixel));
  print(text);
}

void DisplayContentDrawer::drawTextFitted(const char* text, LEDPixel pixel) {
  setTextSize(displayBuffer.getHeight() >= 16 ? 2 : 1);
  drawText(0, 0, text, pixel);
}

uint16_t DisplayContentDrawer::rgb565(LEDPixel pixel) const {
  return ((pixel.red & 0xF8) << 8) |
         ((pixel.green & 0xFC) << 3) |
         (pixel.blue >> 3);
}

LEDPixel DisplayContentDrawer::ledPixelFromRgb565(uint16_t color) const {
  const uint8_t red = ((color >> 11) & 0x1F) << 3;
  const uint8_t green = ((color >> 5) & 0x3F) << 2;
  const uint8_t blue = (color & 0x1F) << 3;

  return {red, green, blue};
}
