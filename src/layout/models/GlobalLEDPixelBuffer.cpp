#include "GlobalLEDPixelBuffer.h"

GlobalLEDPixelBuffer::GlobalLEDPixelBuffer(int width, int height)
  : width(width),
    height(height),
    pixels(width * height, {0, 0, 0}) {
}

void GlobalLEDPixelBuffer::clear(LEDPixel pixel) {
  for (LEDPixel& currentPixel : pixels) {
    currentPixel = pixel;
  }
}

void GlobalLEDPixelBuffer::setPixel(int x, int y, LEDPixel pixel) {
  if (!contains(x, y)) {
    return;
  }

  pixels[pixelIndex(x, y)] = pixel;
}

LEDPixel GlobalLEDPixelBuffer::getPixel(int x, int y) const {
  if (!contains(x, y)) {
    return {0, 0, 0};
  }

  return pixels[pixelIndex(x, y)];
}

bool GlobalLEDPixelBuffer::contains(int x, int y) const {
  return x >= 0 && y >= 0 && x < width && y < height;
}

int GlobalLEDPixelBuffer::pixelIndex(int x, int y) const {
  return y * width + x;
}
